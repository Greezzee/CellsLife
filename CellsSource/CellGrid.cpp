#include "CellGrid.h"
#include "BasicCell.h"
#include "Cell.h"
#include "CellCorpse.h"
#include "../CellWall.h"
#include <iostream>
CellGrid::CellGrid() :
		grid_(std::vector<std::vector<BasicCell*>>(GRID_SIZE_Y, std::vector<BasicCell*>(GRID_SIZE_X, nullptr))) , min_per_opt(0) , sun_per_opt(150) {}

void CellGrid::LoadFromFile(const std::string file_name) {
	int ret = 0;
	const char* file_str = file_name.c_str();
	FILE* file = fopen(file_str, "rb");
	if (file == NULL) {
		std::cout << "Failed to open the file \"" << file_name << "\"!";
		return ;
	}

	// Clear the grid
	DeleteAllCells();

	// Get the information about number of each cells' type
	std::size_t alive_cells_n = 0;
	std::size_t cell_corps_n = 0;
	std::size_t cell_walls_n = 0;

	FREAD(ret, &alive_cells_n, sizeof(alive_cells_n), 1, file);
	FREAD(ret, &cell_corps_n, sizeof(cell_corps_n), 1, file);
	FREAD(ret, &cell_walls_n, sizeof(cell_walls_n), 1, file);

	// Load the information about alive cells
	for (auto i = 0; i < alive_cells_n; ++i) {
		cell_data_t data;
		data.type = cell_type_t::ALIVE;
		FREAD(ret, &(data.data.as_alive), sizeof(alive_data_t), 1, file);
		Cell* tmp = new Cell(data.data.as_alive.grid_pos, 0);
		tmp->SetCellData(data);
		SpawnCell_thr1(tmp);
	}
	// Load the information about cell corps
	for (auto i = 0; i < cell_corps_n; ++i) {
		cell_data_t data;
		data.type = cell_type_t::CORPSE;
		FREAD(ret, &(data.data.as_corpse), sizeof(corpse_data_t), 1, file);
		CellCorpse* tmp = new CellCorpse(data.data.as_corpse.grid_pos, 0);
		tmp->SetCellData(data);
		SpawnCell_thr1(tmp);
	}
	// Load the information about cell walls
	for (auto i = 0; i < cell_walls_n; ++i) {
		cell_data_t data;
		data.type = cell_type_t::WALL;
		FREAD(ret, &(data.data.as_wall), sizeof(wall_data_t), 1, file);
		CellWall* tmp = new CellWall(data.data.as_wall.grid_pos);
		tmp->SetCellData(data);
		SpawnCell_thr1(tmp);
	}

	// Close the file
	ret = fclose(file);
	if (ret < 0) {
		std::cout << "Failed to close the file!" << std::endl;
	}

	std::cout << "The grid was loaded from \"" << file_name << "\"" << std::endl;
}

void CellGrid::SaveToFile(const std::string file_name) {
	int ret = 0;
	const char* file_str = file_name.c_str();
	// Open the target file
	FILE* file = fopen(file_str, "wb");
	if (file == NULL) {
		std::cout << "Failed to open/create the file \"" << file_name << "\"!" << std::endl;
		return;
	}

	// Count alive cells, corps and wall cells
	std::size_t cells_count = cells_.size();
	std::size_t alive_cells_n = 0;
	std::size_t cell_corps_n = 0;
	std::size_t cell_walls_n = 0;
	for (auto&& cell : cells_) {
		switch (cell->GetType()) {
		case cell_type_t::ALIVE:
			alive_cells_n++;
			break;
		case cell_type_t::CORPSE:
			cell_corps_n++;
			break;
		case cell_type_t::WALL:
			cell_walls_n++;
			break;
		default:
			throw std::runtime_error("Unknown cell type!");
		}
	}

	// Put these numbers into the begining of this file (3 size_t variables)
	FWRITE(ret, &alive_cells_n, sizeof(alive_cells_n), 1, file);
	FWRITE(ret, &cell_corps_n, sizeof(cell_corps_n), 1, file);
	FWRITE(ret, &cell_walls_n, sizeof(cell_walls_n), 1, file);

	// Put the information about alive cells
	for (auto& cell : cells_) {
		if (cell->GetType() == cell_type_t::ALIVE) {
			cell_data_t data = cell->GetCellData();
			FWRITE(ret, &(data.data.as_alive), sizeof(alive_data_t), 1, file);
		}
	}
	// Put the information about cell corps
	for (auto& cell : cells_) {
		if (cell->GetType() == cell_type_t::CORPSE) {
			cell_data_t data = cell->GetCellData();
			FWRITE(ret, &(data.data.as_corpse), sizeof(corpse_data_t), 1, file);
		}
	}
	// Put the information about cell walls
	for (auto& cell : cells_) {
		if (cell->GetType() == cell_type_t::WALL) {
			cell_data_t data = cell->GetCellData();
			FWRITE(ret, &(data.data.as_wall), sizeof(wall_data_t), 1, file);
		}
	}

	// Close the file
	ret = fclose(file);
	if (ret < 0) {
		std::cout << "Failed to close the file!" << std::endl;
	}

	std::cout << "The grid was saved to \"" << file_name << "\"" << std::endl;
}

void CellGrid::SpawnCell_thr1(BasicCell* cell) {
	auto pos = cell->GetGridPos();
	if (pos.y >= 0 && pos.y < GRID_SIZE_Y) {
		if (cells_.find(cell) == cells_.end()) {
			cell->SetGrid(this);
			cells_.insert(cell);
			DeleteCell(grid_[pos.y][pos.x % GRID_SIZE_X]);
			grid_[pos.y][pos.x % GRID_SIZE_X] = cell;
			return;
		}
		else {
			std::cout << "Clone!!\n";
			delete cell;
		}
	}
	else {
		std::cout << "Out of grid or at same pos!\n";
		delete cell;
	}
}

std::mutex check_spawn;
void CellGrid::SpawnCell(BasicCell* cell) {
	check_spawn.lock();
	auto pos = cell->GetGridPos();
	if (pos.y >= 0 && pos.y < GRID_SIZE_Y) {
		if (cells_.find(cell) == cells_.end()) {
			cell->SetGrid(this);
			cells_.insert(cell);
			DeleteCell(grid_[pos.y][pos.x % GRID_SIZE_X]);
			grid_[pos.y][pos.x % GRID_SIZE_X] = cell;
		}
		else {
			std::cout << "Clone!!\n";
			delete cell;
		}
	}
	else {
		std::cout << "Out of grid or at same pos!\n";
		delete cell;
	}
	check_spawn.unlock();
}

void CellGrid::DeleteCell(Vector2I pos) {
	auto cell = grid_[pos.y][pos.x];
	DeleteCell(cell);
}

int aaa = 0;
std::mutex check;
void CellGrid::DeleteCell(BasicCell* cell) {
	check.lock();
	
	if (cell != nullptr) {
		cell->Delete();
		cells_to_delete_.push_back(cell);
		grid_[cell->GetGridPos().y][cell->GetGridPos().x] = nullptr;
		aaa++;
	}

	check.unlock();
}

/*void CellGrid::UpdateCells() {
	for (auto& cell : cells_)
		if (!cell->IsToDelete())
			cell->Update();
	ClearDeletedCells();
}*/

void CellGrid::UpdateCells_1()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x > GRID_SIZE_X / 2 + 1) && ((cell->GetGridPos()).y > GRID_SIZE_Y / 2 + 1) && ((cell->GetGridPos()).x < GRID_SIZE_X - 1) && ((cell->GetGridPos()).y < GRID_SIZE_Y - 1))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_2()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x > GRID_SIZE_X / 2 + 1) && ((cell->GetGridPos()).y < GRID_SIZE_Y / 2 - 1) && ((cell->GetGridPos()).x < GRID_SIZE_X - 1) && ((cell->GetGridPos()).y > 0 + 1))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_3()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x < GRID_SIZE_X / 2 - 1) && ((cell->GetGridPos()).y < GRID_SIZE_Y / 2 - 1) && ((cell->GetGridPos()).x > 0 + 1) && ((cell->GetGridPos()).y > 0 + 1))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_4()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x < GRID_SIZE_X / 2 - 1) && ((cell->GetGridPos()).y > GRID_SIZE_Y / 2 + 1) && ((cell->GetGridPos()).x > 0 + 1) && ((cell->GetGridPos()).y < GRID_SIZE_Y - 1))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_cross()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x == GRID_SIZE_X / 2) || ((cell->GetGridPos()).y == GRID_SIZE_Y / 2) || ((cell->GetGridPos()).x == 0) || ((cell->GetGridPos()).y == 0)
			|| ((cell->GetGridPos()).x == GRID_SIZE_X / 2 + 1) || ((cell->GetGridPos()).y == GRID_SIZE_Y / 2 + 1) || ((cell->GetGridPos()).x == 0 + 1) || ((cell->GetGridPos()).y == 0 + 1)
			|| ((cell->GetGridPos()).x == GRID_SIZE_X / 2 - 1) || ((cell->GetGridPos()).y == GRID_SIZE_Y / 2 - 1) || ((cell->GetGridPos()).x == 0 - 1) || ((cell->GetGridPos()).y == 0 - 1)
			|| ((cell->GetGridPos()).x == GRID_SIZE_X) || ((cell->GetGridPos()).y == GRID_SIZE_Y)
			|| ((cell->GetGridPos()).x == GRID_SIZE_X + 1) || ((cell->GetGridPos()).y == GRID_SIZE_Y + 1)
			|| ((cell->GetGridPos()).x == GRID_SIZE_X - 1) || ((cell->GetGridPos()).y == GRID_SIZE_Y - 1))
			if (!cell->IsToDelete())
				cell->Update();
	}
}


void CellGrid::UpdateCells()
{
	std::thread thr_1(&CellGrid::UpdateCells_1 , this);
	std::thread thr_2(&CellGrid::UpdateCells_2 , this);
	std::thread thr_3(&CellGrid::UpdateCells_3 , this);
	std::thread thr_4(&CellGrid::UpdateCells_4 , this);
	thr_1.join();
	thr_2.join();
	thr_3.join();
	thr_4.join();

	UpdateCells_cross();

	ClearDeletedCells();
}

void CellGrid::DrawCells() {
	for (auto& cell : cells_)
		if (!cell->IsToDelete())
			cell->Draw();
}

void CellGrid::DeleteAllCells() {
	for (auto& cell : cells_) {
		cell->Destroy();
		delete cell;
	}
	cells_.clear();
	for (auto& line : grid_)
		for (auto& elem : line)
			elem = nullptr;
}

CellGrid::~CellGrid() {
	for (auto& cell : cells_) {
		cell->Destroy();
		delete cell;
	}
}

BasicCell* CellGrid::GetCellFromGrid(Vector2I pos) {
	if (pos.x < 0)
		pos.x = GRID_SIZE_X - 1;
	if (pos.x >= GRID_SIZE_X)
		pos.x = 0;
	if (pos.y < 0)
		pos.y = 0;
	if (pos.y >= GRID_SIZE_Y)
		pos.y = GRID_SIZE_Y - 1;
	return grid_[pos.y][pos.x];
}

void CellGrid::SetCellToGrid(BasicCell* cell, Vector2I pos) {
	grid_[pos.y][pos.x] = cell;
}

void CellGrid::ClearDeletedCells() {
	//std::cout << "Deleted: " << aaa << ". Cleared: " << cells_to_delete_.size() << "\n";
	if (aaa != cells_to_delete_.size())
		std::cout << "OOPS\n";
	aaa = 0;
	for (auto& cell : cells_to_delete_) {
		auto cell_it = cells_.find(cell);
		cells_.erase(cell_it);
		cell->Destroy();
		delete cell;
	}
	cells_to_delete_.clear();
}