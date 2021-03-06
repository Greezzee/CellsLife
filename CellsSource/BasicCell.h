#pragma once
#include "../Engine/AllEngine.h"
#include "Gen.h"
#include "CellGrid.h"
#include "CellData.h"
#include <array>

class BasicCell : public GameObject
{
public:
	BasicCell::BasicCell(Vector2I start_pos, float start_energy, bool is_eatable) :
		grid_pos_(Vector2I(start_pos.x % GRID_SIZE_X, start_pos.y)), energy_(start_energy), my_grid_(), is_eatable_(is_eatable) {}
	Vector2I GetGridPos() { return grid_pos_; }
	void SetGrid(CellGrid* grid) { my_grid_ = grid; }
	float GetEnergy() { return energy_; }
	cell_type_t GetType() { return my_type_; }
	Color GetFamilyColor() { return family_color_; }
	bool IsToDelete() { return to_delete_; }
	void Delete() { to_delete_ = true; }

	virtual cell_data_t GetCellData() = 0;
	virtual void SetCellData(const cell_data_t& data) = 0;

	bool IsEatable() { return is_eatable_; }
	virtual ~BasicCell() {}
protected:
	Vector2I grid_pos_;
	float energy_;
	CellGrid* my_grid_;
	bool to_delete_ = false;
	Color color_;
	Color family_color_;
	bool is_eatable_ = false;
	cell_type_t my_type_;

	Vector2I GetNearPos(direction dir) {
		auto pos = grid_pos_;
		switch (dir)
		{
		case direction::UP:
			pos.y -= 1;
			if (pos.y < 0)
				pos.y = 0;
			break;
		case direction::DOWN:
			pos.y += 1;
			if (pos.y >= GRID_SIZE_Y)
				pos.y = GRID_SIZE_Y - 1;
			break;
		case direction::LEFT:
			pos.x -= 1;
			if (pos.x < 0)
				pos.x = GRID_SIZE_X - 1;
			break;
		case direction::RIGHT:
			pos.x += 1;
			if (pos.x >= GRID_SIZE_X)
				pos.x = 0;
			break;
		default:
			break;
		}
		return pos;
	}

	void Move(direction dir) {
		auto old_pos = grid_pos_;
		grid_pos_ = GetNearPos(dir);
		if (my_grid_->GetCellFromGrid(grid_pos_) == nullptr) {
			my_grid_->SetCellToGrid(this, grid_pos_);
			my_grid_->SetCellToGrid(nullptr, old_pos);
		}
		else
			grid_pos_ = old_pos;
	}
};