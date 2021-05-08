#pragma once

#include "CellsSource/BasicCell.h"

class CellWall : public BasicCell {
public:
	CellWall(Vector2I start_pos) :
		BasicCell(start_pos, 0, false) {
		family_color_ = Color::Black();
		my_type_ = cell_type_t::WALL;
	}

	void Update() override {}
	void Draw() override { Debugger::DrawPoint(Vector2F(grid_pos_.x * 4, grid_pos_.y * 4), 4, 0, Color::White()); }
	void Destroy() override {}
};