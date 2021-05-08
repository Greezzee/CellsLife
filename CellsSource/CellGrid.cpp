#include "CellGrid.h"
#include "BasicCell.h"
#include <iostream>
CellGrid::CellGrid() :
	grid_(std::vector<std::vector<BasicCell*>>(GRID_SIZE_Y, std::vector<BasicCell*>(GRID_SIZE_X, nullptr))) {}

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
		if (((cell->GetGridPos()).x > GRID_SIZE_X / 2) && ((cell->GetGridPos()).y > GRID_SIZE_Y / 2) && ((cell->GetGridPos()).x < GRID_SIZE_X) && ((cell->GetGridPos()).y < GRID_SIZE_Y))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_2()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x > GRID_SIZE_X / 2) && ((cell->GetGridPos()).y < GRID_SIZE_Y / 2) && ((cell->GetGridPos()).x < GRID_SIZE_X) && ((cell->GetGridPos()).y > 0))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_3()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x < GRID_SIZE_X / 2) && ((cell->GetGridPos()).y < GRID_SIZE_Y / 2) && ((cell->GetGridPos()).x > 0) && ((cell->GetGridPos()).y > 0))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_4()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x < GRID_SIZE_X / 2) && ((cell->GetGridPos()).y > GRID_SIZE_Y / 2) && ((cell->GetGridPos()).x > 0) && ((cell->GetGridPos()).y < GRID_SIZE_Y))
			if (!cell->IsToDelete())
				cell->Update();
	}
}

void CellGrid::UpdateCells_cross()
{
	for (auto& cell : cells_)
	{
		if (((cell->GetGridPos()).x == GRID_SIZE_X / 2) || ((cell->GetGridPos()).y == GRID_SIZE_Y / 2) || ((cell->GetGridPos()).x == 0) || ((cell->GetGridPos()).y == 0))
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