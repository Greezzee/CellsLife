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