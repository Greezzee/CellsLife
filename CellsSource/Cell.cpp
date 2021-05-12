#include "Cell.h"
#include "BasicCell.h"
#include "CellGrid.h"
#include "CellCorpse.h"
#include "Gen.h"

unsigned Randomize256(unsigned x) {
	unsigned r = rand() % 25;
	switch (rand() % 2)
	{
	case 0:
		if (x < 255 - r)
			return x + r;
		else
			return x;
		break;
	case 1:
		if (x < r)
			return x;
		else
			return x - r;
		break;
	default:
		return 0;
		break;
	}
}

bool IsSameWithError(unsigned a, unsigned b, unsigned error) {
	unsigned A = std::min(a, b);
	unsigned B = std::max(a, b);
	return B - A <= error;
}

Cell::Cell(Vector2I start_pos, float start_energy) :
	BasicCell(start_pos, start_energy, true), Beh_DNA_(), Prop_DNA_() {
	for (auto& gen : Beh_DNA_)
		gen = { gen_order::EAT_SUN, direction::UP, 0, 0 };
	for (auto& gen : Prop_DNA_)
		gen = property_gen_t::MUTATION_RATE;
	family_color_ = Color::Green();
	my_type_ = cell_type_t::ALIVE;
}

Cell::Cell(Vector2I start_pos, float start_energy, const std::array<Gen, BEH_DNA_SIZE>& parent_DNA, const std::array<property_gen_t, PROP_DNA_SIZE>& parent_prop_DNA, parent_color_t parent_color) :
	BasicCell(start_pos, start_energy, true) {
	Beh_DNA_ = parent_DNA;
	Prop_DNA_ = parent_prop_DNA;

	for (auto& gen : Prop_DNA_) {
		if (gen == property_gen_t::MUTATION_RATE)
			mutation_chance++;
	}

	for (auto& gen : Beh_DNA_) {
		if (rand() % 100 < mutation_chance)
			gen = { static_cast<gen_order>(std::rand() % static_cast<int>(gen_order::gen_order_count)),
					static_cast<direction>(std::rand() % static_cast<int>(direction::direction_count)),
					static_cast<char>(std::rand()), static_cast<char>(std::rand()) };
	}
	for (auto& gen : Prop_DNA_) {
		if (rand() % 100 < mutation_chance)
			gen = static_cast<property_gen_t>(std::rand() % static_cast<int>(property_gen_t::property_gen_count));
	}

	for (auto& gen : Prop_DNA_) {
		switch (gen)
		{
		case property_gen_t::SPEED: actions_per_step++; break;
		case property_gen_t::MAX_HEALTH: max_energy += 10.f; break;
		case property_gen_t::SUN_EFF: 
			efficenty_.sun_eff += 0.05f;
			efficenty_.minerals_eff -= 0.05f / 2.f;
			efficenty_.predator_eff -= 0.015f / 2.f;
			efficenty_.corpse_eff -= 0.05f / 2.f;
			break;
		case property_gen_t::MINERALS_EFF: 
			efficenty_.sun_eff -= 0.05f / 2.f;
			efficenty_.minerals_eff += 0.05f;
			efficenty_.predator_eff -= 0.015f / 2.f;
			efficenty_.corpse_eff -= 0.05f / 2.f;
			break;
		case property_gen_t::PREDATOR_EFF: 
			efficenty_.sun_eff -= 0.05f / 2.f;
			efficenty_.minerals_eff -= 0.05f / 2.f;
			efficenty_.predator_eff += 0.015f;
			efficenty_.corpse_eff -= 0.05f / 2.f;
			break;
		case property_gen_t::CORPSE_EFF: 
			efficenty_.sun_eff -= 0.05f / 2.f;
			efficenty_.minerals_eff -= 0.05f / 2.f;
			efficenty_.predator_eff -= 0.015f / 2.f;
			efficenty_.corpse_eff += 0.05f;
			break;
		default:
			break;
		}
	}

	family_color_ = parent_color.family_color;
	color_ = parent_color.beh_color;
	
	if (rand() % 100 < mutation_chance * 5) {
		family_color_.r = Randomize256(family_color_.r);
		family_color_.g = Randomize256(family_color_.g);
		family_color_.b = Randomize256(family_color_.b);
	}
	my_type_ = cell_type_t::ALIVE;
}

void Cell::Update() {
	float ener;
	Vector2I buffer_pos;
	BasicCell* buf_cell;
	bool rage_flag = is_rage_;
	is_divided_ = false;
	int action_count = 0;
	while (action_count < actions_per_step) {
		switch (Beh_DNA_[current_beh_gen].ID)
		{
		case gen_order::DO_NOTHING:
			break;
		case gen_order::GO:
			action_count += 5;
			if (!is_rage_) {
				Move(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
			}
			else {
				buffer_pos = GetNearPos(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
				if (buffer_pos == grid_pos_)
					break;
				if ((buf_cell = my_grid_->GetCellFromGrid(buffer_pos)) == nullptr) {
					Move(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
					break;
				}
				if (!buf_cell->IsEatable())
					break;
				ener = buf_cell->GetEnergy();
				my_grid_->DeleteCell(buf_cell);
				Move(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
				ener *= 0.5f * efficenty_.predator_eff;
				energy_ += ener;
				if (color_.r + ener <= 255u) color_.r += ener;
				if (color_.g >= ener * 0.5f) color_.g -= ener * 0.5f;
				if (color_.b >= ener * 0.5f) color_.b -= ener * 0.5f;
				is_rage_ = false;
				action_count += actions_per_step;
			}
			break;
		case gen_order::EAT_SUN:
			action_count += actions_per_step;
			ener = my_grid_->GetSun(grid_pos_) * efficenty_.sun_eff;
			energy_ += ener;
			if (color_.g + ener <= 255u) color_.g += ener;
			if (color_.b >= ener * 0.5f) color_.b -= ener * 0.5f;
			if (color_.r >= ener * 0.5f) color_.r -= ener * 0.5f;
			break;
		case gen_order::EAT_MINERALS:
			action_count += actions_per_step;
			ener = my_grid_->GetMinerals(grid_pos_) * efficenty_.minerals_eff;
			energy_ += ener;
			if (color_.b + ener <= 255u) color_.b += ener;
			if (color_.g >= ener * 0.5f) color_.g -= ener * 0.5f;
			if (color_.r >= ener * 0.5f) color_.r -= ener * 0.5f;
			break;
		case gen_order::BECOME_RAGE:
			action_count++;
			is_rage_ = true;
			break;
		case gen_order::ROTATE:
			action_count++;
			rotation_ = TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_);
			break;
		case gen_order::JUMP:
			action_count++;
			current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_CELL_NEAR:
			action_count++;
			buffer_pos = GetNearPos(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
			if (my_grid_->GetCellFromGrid(buffer_pos) == nullptr)
				break;
			current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_ALIVE_NEAR:
			action_count++;
			buffer_pos = GetNearPos(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
			if (my_grid_->GetCellFromGrid(buffer_pos) == nullptr || my_grid_->GetCellFromGrid(buffer_pos)->GetType() != cell_type_t::ALIVE)
				break;
			current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_CORPSE_NEAR:
			action_count++;
			buffer_pos = GetNearPos(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
			if (my_grid_->GetCellFromGrid(buffer_pos) == nullptr || my_grid_->GetCellFromGrid(buffer_pos)->GetType() != cell_type_t::CORPSE)
				break;
			current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_RELATIVE_NEAR:
			action_count++;
			buffer_pos = GetNearPos(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
			if (my_grid_->GetCellFromGrid(buffer_pos) == nullptr)
				break;
			if (!IsSameWithError(family_color_.r, my_grid_->GetCellFromGrid(buffer_pos)->GetFamilyColor().r, 10))
				break;
			if (!IsSameWithError(family_color_.g, my_grid_->GetCellFromGrid(buffer_pos)->GetFamilyColor().g, 10))
				break;
			if (!IsSameWithError(family_color_.b, my_grid_->GetCellFromGrid(buffer_pos)->GetFamilyColor().b, 10))
				break;
			current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_ENERGY_LESS:
			action_count++;
			if (energy_ < static_cast<float>(Beh_DNA_[current_beh_gen].additional_data))
				current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_ENERGY_HIGHER:
			action_count++;
			if (energy_ > static_cast<float>(Beh_DNA_[current_beh_gen].additional_data))
				current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_Y_HIGHER:
			action_count++;
			if (grid_pos_.y > static_cast<float>(Beh_DNA_[current_beh_gen].additional_data))
				current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::JUMP_IF_Y_LESS:
			action_count++;
			if (grid_pos_.y < static_cast<float>(Beh_DNA_[current_beh_gen].additional_data))
				current_beh_gen = Beh_DNA_[current_beh_gen].data % BEH_DNA_SIZE;
			break;
		case gen_order::DIVIDE:
			action_count += actions_per_step;
			if (energy_ >= 30.f)
				BornCell();
			break;
		case gen_order::EAT_CORPSE:
			action_count += actions_per_step;
			buffer_pos = GetNearPos(TransformWithRotation(Beh_DNA_[current_beh_gen].dir, rotation_));
			if (buffer_pos == grid_pos_)
				break;
			if ((buf_cell = my_grid_->GetCellFromGrid(buffer_pos)) == nullptr) 
				break;
			if (buf_cell->GetType() != cell_type_t::CORPSE)
				break;
			ener = buf_cell->GetEnergy() * efficenty_.corpse_eff;
			my_grid_->DeleteCell(buf_cell);
			ener = 0.5f * std::abs(ener);
			energy_ += ener;
			if (color_.b + ener * 0.5f <= 255u) color_.b += ener * 0.5f;
			if (color_.g >= ener) color_.g -= ener;
			if (color_.r + ener * 0.5f <= 255u) color_.r += ener * 0.5f;
			break;
		default:
			break;
		}
		current_beh_gen = (current_beh_gen + 1) % BEH_DNA_SIZE;
	}
	energy_ -= energy_per_step;
	energy_per_step += 0.02;
	age_++;
	if (rage_flag)
		is_rage_ = false;
	if ((energy_ < 15.f) && !is_died_) {
		my_grid_->DeleteCell(this);
		my_grid_->SpawnCell(new CellCorpse(grid_pos_, -15));
		is_died_ = true;
	}
	if (energy_ >= max_energy && !is_died_)
		BornCell();
}
void Cell::Draw() {
	switch (my_grid_->GetDrawType())
	{
	case 1:
		Debugger::DrawPoint(Vector2F(grid_pos_.x * 4, grid_pos_.y * 4), 4, 0, color_);
		break;
	case 2:
		Debugger::DrawPoint(Vector2F(grid_pos_.x * 4, grid_pos_.y * 4), 4, 0, family_color_);
		break;
	case 3:
		Debugger::DrawPoint(Vector2F(grid_pos_.x * 4, grid_pos_.y * 4), 4, 0, Color(static_cast<unsigned>(energy_ / max_energy * 255.f), 0, static_cast<unsigned>(255.f - energy_ / max_energy * 255.f)));
		break;
	case 4:
		Debugger::DrawPoint(Vector2F(grid_pos_.x * 4, grid_pos_.y * 4), 4, 0, Color(255 - age_, 0, age_));
	default:
		break;
	}
}
void Cell::Destroy() {

}

void Cell::BornCell() {
	is_divided_ = true;
	energy_ /= 2;
	auto new_pos = grid_pos_;
	if (grid_pos_.y > 0 && my_grid_->GetCellFromGrid(grid_pos_ + Vector2I(0, -1)) == nullptr) {
		new_pos.y -= 1;
	}
	else if (grid_pos_.y < GRID_SIZE_Y - 1 && my_grid_->GetCellFromGrid(grid_pos_ + Vector2I(0, +1)) == nullptr) {
		new_pos.y += 1;
	}
	else if (my_grid_->GetCellFromGrid(grid_pos_ + Vector2I(1, 0)) == nullptr) {
		new_pos.x++;
		if (new_pos.x >= GRID_SIZE_X)
			new_pos.x = 0;
	}
	else if (my_grid_->GetCellFromGrid(grid_pos_ + Vector2I(-1, 0)) == nullptr) {
		new_pos.x--;
		if (new_pos.x < 0)
			new_pos.x = GRID_SIZE_X - 1;
	}
	else {
		my_grid_->DeleteCell(this);
		my_grid_->SpawnCell(new CellCorpse(grid_pos_, -energy_));
		is_died_ = true;
		return;
	}
	my_grid_->SpawnCell(new Cell(new_pos, energy_, Beh_DNA_, Prop_DNA_, { family_color_, color_ }));
}