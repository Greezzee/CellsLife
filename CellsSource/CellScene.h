#pragma once
#include "../Engine/AllEngine.h"
#include "CellGrid.h"
#include "Cell.h"
#include "../CellWall.h"
class CellScene : public Scene {
public:
	void Init() override {
		std::srand(time(NULL));
		//GraphicManager::ShowFPS(true);
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

		if (InputManager::IsDown(MouseKey::Mouse_Left)) {
			/*
			Vector2I mouse_pos = Vector2I(static_cast<int>(InputManager::GetMousePos().x) / 4, static_cast<int>(InputManager::GetMousePos().y) / 4);
			//grid_.SpawnCell(new CellWall(mouse_pos));
			BasicCell* cell = grid_.GetCellFromGrid(mouse_pos);
			Cell* c;
			if ((c = dynamic_cast<Cell*>(cell)) != nullptr) {
				c->PrintDNA();
			}
			*/
			Vector2I mouse_pos = Vector2I(static_cast<int>(InputManager::GetMousePos().x) / 4, static_cast<int>(InputManager::GetMousePos().y) / 4);
			grid_.SpawnCell_thr1(new CellWall(mouse_pos));
		}

		if (InputManager::IsDown(MouseKey::Mouse_Right)) {
			Vector2I mouse_pos = Vector2I(static_cast<int>(InputManager::GetMousePos().x) / 4, static_cast<int>(InputManager::GetMousePos().y) / 4);
			grid_.SpawnCell_thr1(new Cell(mouse_pos, 50.f));
		}

		if (InputManager::IsPressed(KeyboardKey::R)) {
			grid_.DeleteAllCells();
		}

		if (InputManager::IsPressed(KeyboardKey::S)) {
			std::string input;
			std::cout << "Enter the name of the file: ";
			std::cin >> input;
			grid_.SaveToFile(input);
		}

		if (InputManager::IsPressed(KeyboardKey::L)) {
			std::string input;
			std::cout << "Enter the name of the file: ";
			std::cin >> input;
			grid_.LoadFromFile(input);
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

		if (InputManager::IsPressed(N3)) {
			grid_.SetDrawType(3);
		}

		if (InputManager::IsPressed(N4)) {
			grid_.SetDrawType(4);
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

struct sprite_info
{
	unsigned spriteID;
	unsigned spriteLR;
	Vector2F sprite_pos;
	Vector2F size;

	sprite_info(unsigned ID, unsigned LR, Vector2F size_) : spriteID(ID), spriteLR(LR)
	{
		size.x = size_.x;
		size.y = size_.y;
		sprite_pos.x = 0;
		sprite_pos.y = 0;
	}
};

/*class OptScene : public Scene
{
private:
	std::vector<sprite_info> sprites_info;
	void load_sprites()
	{

	}

	void draw_sprites()
	{

	}

public:

	void Init() override
	{
		std::srand(time(NULL));
		GraphicManager::ShowFPS(NULL);

		load_sprites();
	}

	void Update() override
	{

	}


};*/

enum buttons
{
	START , START_PRESSED , OPT , OPT_PRESSED , BACK_ , BACK_PRESSED
};

class StartScene : public Scene
{

private:
	std::vector<sprite_info> sprites_info;
	void load_sprites()
	{
		int texID = GraphicManager::LoadTexture("./tests/fullbuttonsprite.png");
		//std::cout << texID << std::endl;
		Vector2F size = { 731 , 256 };
		Vector2F size_spr = { 600 , 200 };
		Vector2F pos_tx = { 0 , 0 };
		Vector2F pos_fl = { 280 , 10 };
		for (int i = 0; i < 6; ++i)
		{
			GraphicPrefabData sprite(texID, size, 1, pos_tx);
			int spr_ID = GraphicManager::LoadSprite(sprite);
			sprites_info.push_back(sprite_info(spr_ID , 10 , size_spr));
			pos_tx.x += 731;
		}

			sprites_info[START].sprite_pos.x = pos_fl.x;
			sprites_info[START_PRESSED].sprite_pos.x = pos_fl.x;
			sprites_info[START].sprite_pos.y = pos_fl.y;
			sprites_info[START_PRESSED].sprite_pos.y = pos_fl.y;

			pos_fl.y += 220;

			sprites_info[OPT].sprite_pos.x = pos_fl.x;
			sprites_info[OPT_PRESSED].sprite_pos.x = pos_fl.x;
			sprites_info[OPT].sprite_pos.y = pos_fl.y;
			sprites_info[OPT_PRESSED].sprite_pos.y = pos_fl.y;

			pos_fl.y += 220;

			sprites_info[BACK_].sprite_pos.x = pos_fl.x;
			sprites_info[BACK_PRESSED].sprite_pos.x = pos_fl.x;
			sprites_info[BACK_].sprite_pos.y = pos_fl.y;
			sprites_info[BACK_PRESSED].sprite_pos.y = pos_fl.y;
	}
	
public:
	void Init() override
	{
		std::srand(time(NULL));
		GraphicManager::ShowFPS(NULL);

		load_sprites();
	}

	void Update() override
	{

		if ((InputManager::GetMousePos().x > sprites_info[START].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[START].sprite_pos.x + sprites_info[START].size.x) &&
			(InputManager::GetMousePos().y > sprites_info[START].sprite_pos.y) && (InputManager::GetMousePos().y < sprites_info[START].sprite_pos.y + sprites_info[START].size.y))
		{
			DrawData for_draw(sprites_info[START_PRESSED].spriteID, sprites_info[START_PRESSED].spriteLR, sprites_info[START_PRESSED].sprite_pos);
			for_draw.size.x = 600;
			for_draw.size.y = 200;
			GraphicManager::Draw(for_draw);
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				Scene* start = new CellScene;
				SceneManager::CreateScene(start);
				SceneManager::CloseScene(this);
				return;
			}
		}
		else
		{
			DrawData for_draw(sprites_info[START].spriteID, sprites_info[START].spriteLR, sprites_info[START].sprite_pos);
			for_draw.size.x = 600;
			for_draw.size.y = 200;
			GraphicManager::Draw(for_draw);
		}

		if ((InputManager::GetMousePos().x > sprites_info[OPT].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[OPT].sprite_pos.x + sprites_info[OPT].size.x) &&
			(InputManager::GetMousePos().y > sprites_info[OPT].sprite_pos.y) && (InputManager::GetMousePos().y < sprites_info[OPT].sprite_pos.y + sprites_info[OPT].size.y))
		{
			DrawData for_draw(sprites_info[OPT_PRESSED].spriteID, sprites_info[OPT_PRESSED].spriteLR, sprites_info[OPT_PRESSED].sprite_pos);
			for_draw.size.x = 600;
			for_draw.size.y = 200;
			GraphicManager::Draw(for_draw);
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				/*Scene* start = new OptScene;
				SceneManager::CreateScene(start);
				SceneManager::CloseScene(this);*/
				//return;
			}
		}
		else
		{
			DrawData for_draw(sprites_info[OPT].spriteID, sprites_info[OPT].spriteLR, sprites_info[OPT].sprite_pos);
			for_draw.size.x = 600;
			for_draw.size.y = 200;
			GraphicManager::Draw(for_draw);
		}

		if ((InputManager::GetMousePos().x > sprites_info[BACK_].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[BACK_].sprite_pos.x + sprites_info[BACK_].size.x) &&
			(InputManager::GetMousePos().y > sprites_info[BACK_].sprite_pos.y) && (InputManager::GetMousePos().y < sprites_info[BACK_].sprite_pos.y + sprites_info[BACK_].size.y))
		{
			DrawData for_draw(sprites_info[BACK_PRESSED].spriteID, sprites_info[BACK_PRESSED].spriteLR, sprites_info[BACK_PRESSED].sprite_pos);
			for_draw.size.x = 600;
			for_draw.size.y = 200;
			GraphicManager::Draw(for_draw);
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				SceneManager::CloseScene(this);
				return;
			}
		}
		else
		{
			DrawData for_draw(sprites_info[BACK_].spriteID, sprites_info[BACK_].spriteLR, sprites_info[BACK_].sprite_pos);
			for_draw.size.x = 600;
			for_draw.size.y = 200;
			GraphicManager::Draw(for_draw);
		}
	}

	void Destroy() override 
	{

	}
};