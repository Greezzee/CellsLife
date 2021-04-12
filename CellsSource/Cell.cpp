#include "Cell.h"
#include "BasicCell.h"
#include "CellGrid.h"
#include "CellCorpse.h"

Cell::Cell(Vector2I start_pos, float start_energy) :
	BasicCell(start_pos, start_energy, true), Beh_DNA_() {
	for (auto& gen : Beh_DNA_)
		gen = { gen_order::EAT_SUN, direction::UP };
}

Cell::Cell(Vector2I start_pos, float start_energy, const std::array<Gen, BEH_DNA_SIZE>& parent_DNA) :
	BasicCell(start_pos, start_energy, true) {
	Beh_DNA_ = parent_DNA;
	for (auto& gen : Beh_DNA_) {
		if (rand() % 1000 < 1)
			gen = { static_cast<gen_order>(std::rand() % static_cast<int>(gen_order::gen_order_count)),
					static_cast<direction>(std::rand() % static_cast<int>(direction::direction_count)) };
	}
}

void Cell::Update() {
	float ener;
	Vector2I buffer_pos;
	BasicCell* buf_cell;
	switch (Beh_DNA_[current_beh_gen].ID)
	{
	case gen_order::DO_NOTHING:
		break;
	case gen_order::GO:
		if (!is_rage_) {
			Move(Beh_DNA_[current_beh_gen].dir);
		}
		else {
			buffer_pos = GetNearPos(Beh_DNA_[current_beh_gen].dir);
			if (buffer_pos == grid_pos_)
				break;
			if ((buf_cell = my_grid_->GetCellFromGrid(buffer_pos)) == nullptr) {
				Move(Beh_DNA_[current_beh_gen].dir);
				break;
			}
			if (!buf_cell->IsEatable())
				break;
			ener = buf_cell->GetEnergy() / 2.f;
			my_grid_->DeleteCell(buf_cell);
			Move(Beh_DNA_[current_beh_gen].dir);
			energy_ += ener;
			if (color_.r + ener * 2 <= 255u) color_.r += ener * 2;
			if (color_.g >= ener * 2) color_.g -= ener * 2;
			if (color_.b >= ener * 2) color_.b -= ener * 2;
			is_rage_ = false;
		}
		break;
	case gen_order::EAT_SUN:
		ener = my_grid_->GetSun(grid_pos_);
		energy_ += ener;
		if (color_.g + ener * 2 <= 255u) color_.g += ener * 2;
		if (color_.b >= ener * 2) color_.b -= ener * 2;
		if (color_.r >= ener * 2) color_.r -= ener * 2;
		break;
	case gen_order::EAT_MINERALS:
		ener = my_grid_->GetMinerals(grid_pos_);
		energy_ += ener;
		if (color_.b + ener * 2 <= 255u) color_.b += ener * 2;
		if (color_.g >= ener * 2) color_.g -= ener * 2;
		if (color_.r >= ener * 2) color_.r -= ener * 2;
		break;
	case gen_order::BECOME_RAGE:
		is_rage_ = true;
		break;
	default:
		break;
	}
	current_beh_gen = (current_beh_gen + 1) % BEH_DNA_SIZE;
	energy_--;
	if (energy_ < 0)
		my_grid_->DeleteCell(this);
	if (energy_ >= 150.f)
		BornCell();
}
void Cell::Draw() {
	Debugger::DrawPoint(Vector2F(grid_pos_.x * 4, grid_pos_.y * 4), 4, 0, color_);
}
void Cell::Destroy() {

}

void Cell::BornCell() {
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
			new_pos.x = GRID_SIZE_X - 1;
	}
	else if (my_grid_->GetCellFromGrid(grid_pos_ + Vector2I(-1, 0)) == nullptr) {
		new_pos.x--;
		if (new_pos.x < 0)
			new_pos.x = 0;
	}
	else {
		my_grid_->DeleteCell(this);
		my_grid_->SpawnCell(new CellCorpse(grid_pos_, 50.f));
		return;
	}
	my_grid_->SpawnCell(new Cell(new_pos, energy_, Beh_DNA_));
}