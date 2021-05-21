#include "CellCorpse.h"
#include "BasicCell.h"

CellCorpse::CellCorpse(Vector2I start_pos, float start_energy) :
	BasicCell(start_pos, start_energy, false) {
	my_type_ = cell_type_t::CORPSE;
	family_color_ = Color::Black();
}

void CellCorpse::Update() {
	corpse_timer++;
	if (corpse_timer >= max_time) {
		//my_grid_->DeleteCell(this);
		return;
	}
	Move(direction::DOWN);
}
void CellCorpse::Draw() {
	Debugger::DrawPoint(Vector2F(grid_pos_.x * 4, grid_pos_.y * 4), 4, 0, Color::Black());
}
void CellCorpse::Destroy() {}

cell_data_t CellCorpse::GetCellData() {
	cell_data_t out;
	out.type = my_type_;
	out.data.as_corpse.corpse_timer = corpse_timer;
	out.data.as_corpse.energy = energy_;
	out.data.as_corpse.grid_pos = grid_pos_;

	return out;
}

void CellCorpse::SetCellData(const cell_data_t& data) {
	my_type_ = cell_type_t::CORPSE;
	corpse_timer = data.data.as_corpse.corpse_timer;
	energy_ = data.data.as_corpse.energy;
	grid_pos_ = data.data.as_corpse.grid_pos;
}