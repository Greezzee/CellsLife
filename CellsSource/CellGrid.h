#pragma once
#include <unordered_set>
#include <array>
#include "../Engine/AllEngine.h"

class BasicCell;

const size_t GRID_SIZE_X = 200;
const size_t GRID_SIZE_Y = 150;

class CellGrid final {
public:
	CellGrid();

	void SpawnCell(BasicCell* cell);
	void DeleteCell(Vector2I pos);
	void DeleteCell(BasicCell* cell);
	void UpdateCells();
	void DrawCells();

	~CellGrid();

	BasicCell* GetCellFromGrid(Vector2I pos);
	void SetCellToGrid(BasicCell* cell, Vector2I pos);

	float GetSun(Vector2I pos) {
		return std::max(0.f, static_cast<float>(125 - pos.y) / 25.f);
	}
	float GetMinerals(Vector2I pos) {
		return std::max(0.f, static_cast<float>(pos.y - 75) / 25.f);
	}
private:
	std::vector<std::vector<BasicCell*>> grid_;
	std::unordered_set<BasicCell*> cells_;
	std::vector<BasicCell*> cells_to_delete_;

	void ClearDeletedCells();
};