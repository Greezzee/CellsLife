#pragma once
#include "../Engine/AllEngine.h"
#include "CellGrid.h"
#include "Cell.h"
#include "../CellWall.h"
class CellScene : public Scene {
public:
	void Init() override {
		std::srand(time(NULL));
		GraphicManager::ShowFPS(true);
		//grid_.SpawnCell(new Cell({10, 10}, 50.f));
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

		if (InputManager::IsPressed(MouseKey::Mouse_Left)) {
			Vector2I mouse_pos = Vector2I(static_cast<int>(InputManager::GetMousePos().x) / 4, static_cast<int>(InputManager::GetMousePos().y) / 4);
			//grid_.SpawnCell(new CellWall(mouse_pos));
			BasicCell* cell = grid_.GetCellFromGrid(mouse_pos);
			Cell* c;
			if ((c = dynamic_cast<Cell*>(cell)) != nullptr) {
				c->PrintDNA();
			}
		}

		if (InputManager::IsDown(MouseKey::Mouse_Right)) {
			Vector2I mouse_pos = Vector2I(static_cast<int>(InputManager::GetMousePos().x) / 4, static_cast<int>(InputManager::GetMousePos().y) / 4);
			grid_.SpawnCell_thr1(new Cell(mouse_pos, 50.f));
		}

		if (InputManager::IsPressed(KeyboardKey::R)) {
			grid_.DeleteAllCells();
		}

		if (InputManager::IsPressed(KeyboardKey::SPACE)) {
			is_paused = !is_paused;
		}

		if (InputManager::IsPressed(N1)) {
			grid_.SetDrawType(1);
		}

		if (InputManager::IsPressed(N2)) {
			grid_.SetDrawType(2);
		}

		if (!is_paused)
			grid_.UpdateCells();
		grid_.DrawCells();
	}

	void Destroy() override {
	}

private:
	CellGrid grid_;
	bool is_paused = false;
};