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

	cell_data_t GetCellData() {
		cell_data_t out;
		out.type = my_type_;
		out.data.as_wall.grid_pos = grid_pos_;

		return out;
	}
	void SetCellData(const cell_data_t& data) {
		my_type_ = cell_type_t::WALL;
		grid_pos_ = data.data.as_wall.grid_pos;
	}
};