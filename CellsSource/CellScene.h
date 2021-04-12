#pragma once
#include "../Engine/AllEngine.h"
#include "CellGrid.h"
#include "Cell.h"
class CellScene : public Scene {
public:
	void Init() override {
		std::srand(time(NULL));
		GraphicManager::ShowFPS(true);
		grid_.SpawnCell(new Cell({10, 10}, 50.f));
		//grid_.SpawnCell(new Cell({ 11, 10 }, 5000.f));
		//grid_.SpawnCell(new Cell({ 12, 10 }, 5000.f));
		//grid_.SpawnCell(new Cell({ 13, 10 }, 5000.f));
		//grid_.SpawnCell(new Cell({ 14, 10 }, 5000.f));
		//std::array<Gen, BEH_DNA_SIZE> Beh_DNA_;
		//for (int i = 0; i < BEH_DNA_SIZE; i++) {
		//	Beh_DNA_[i] = { gen_order::DO_NOTHING, direction::RIGHT };
		//}
		//Beh_DNA_[0] = { gen_order::BECOME_RAGE, direction::RIGHT };
		//Beh_DNA_[1] = { gen_order::GO, direction::RIGHT };
		//grid_.SpawnCell(new Cell({ 5, 10 }, 5000.f, Beh_DNA_));
	}

	void Update() override {
		Debugger::DrawPoint(Vector2F(0, 0), 5000, 0, Color(50, 50, 50));
		grid_.UpdateCells();
		grid_.DrawCells();
	}

	void Destroy() override {
	}

private:
	CellGrid grid_;
};