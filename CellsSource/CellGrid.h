#pragma once
#include <unordered_set>
#include <array>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream>
#include "../Engine/AllEngine.h"

class BasicCell;

const size_t GRID_SIZE_X = 200;
const size_t GRID_SIZE_Y = 150;

class CellGrid final {
public:
	CellGrid();

	void SpawnCell_thr1(BasicCell* cell);
	void SpawnCell(BasicCell* cell);
	void DeleteCell(Vector2I pos);
	void DeleteCell(BasicCell* cell);
	void UpdateCells_1();
	void UpdateCells_2();
	void UpdateCells_3();
	void UpdateCells_4();
	void UpdateCells_cross();
	void UpdateCells();
	void DrawCells();
	void DeleteAllCells();

	~CellGrid();

	BasicCell* GetCellFromGrid(Vector2I pos);
	void SetCellToGrid(BasicCell* cell, Vector2I pos);

	float GetSun(Vector2I pos) {
		return std::max(0.f, static_cast<float>(150 - pos.y) / 25.f);
	}
	float GetMinerals(Vector2I pos) {
		return std::max(0.f, static_cast<float>(pos.y) / 25.f);
	}

	int GetDrawType() {
		return draw_type_;
	}

	void SetDrawType(int draw_type) {
		draw_type_ = draw_type;
	}

	// Macro for fwrite with error checking
#define FWRITE(ret, buf, size, count, file_stream)						\
	do {																\
		ret = fwrite(buf, size, count, file_stream);					\
		if (ret < 0) {													\
			throw std::runtime_error("Error while saving the grid!");	\
		}																\
	} while (0);														\

	// Macro for fread with error checking
#define FREAD(ret, buf, size, count, file_stream)						\
	do {																\
		ret = fread(buf, size, count, file_stream);						\
		if (ret < 0) {													\
			throw std::runtime_error("Error while reading the file!");	\
		}																\
	} while (0);														\

	// Save the grid into the file with the given name
	void SaveToFile(const std::string file_name);
	// Load the grid from the file with the given name
	void LoadFromFile(const std::string file_name);
private:
	std::vector<std::vector<BasicCell*>> grid_;
	std::unordered_set<BasicCell*> cells_;
	std::vector<BasicCell*> cells_to_delete_;

	int draw_type_ = 1;

	void ClearDeletedCells();
};