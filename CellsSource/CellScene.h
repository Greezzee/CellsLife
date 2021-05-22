#pragma once
#include "../Engine/AllEngine.h"
#include "CellGrid.h"
#include "Cell.h"
#include "../CellWall.h"

int sun_per = 150;
int min_per = 0;

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

class StartScene : public Scene
{
private:
	std::vector<sprite_info> sprites_info;
	void load_sprites();
public:
	void Init() override;
	void Update() override;
	void Destroy() override;

};


class EastScene : public Scene
{
private:
	std::vector<sprite_info> sprites_info;
	int check;
	void load_sprites()
	{
		int texID = GraphicManager::LoadTexture("./tests/CREDITS.png");

		Vector2F pos_tx = { 0 , 0 };
		Vector2F size = { 1024 , 1024 };
		Vector2F size_fl = { 1800 , 1200 };

		GraphicPrefabData sprite(texID, size, 1, pos_tx);
		int spr_ID = GraphicManager::LoadSprite(sprite);
		sprites_info.push_back(sprite_info(spr_ID, 10, size_fl));

		sprites_info[0].sprite_pos.x = 0;
		sprites_info[0].sprite_pos.y = 0;
	}
public:
	void Init() override
	{
		std::srand(time(NULL));
		GraphicManager::ShowFPS(NULL);
		check = 0;
		load_sprites();
	}

	void Update() override
	{
		if (check == 0)
		{
			Debugger::DrawPoint(Vector2F(300, 500), 4, 0, Color::Green());
			Debugger::DrawPoint(InputManager::GetMousePos(), 4, 0, Color::Red());
		}
		else
		{
			DrawData for_draw1(sprites_info[0].spriteID, sprites_info[0].spriteLR, sprites_info[0].sprite_pos);
			for_draw1.size.x = 1250;
			for_draw1.size.y = 700;
			GraphicManager::Draw(for_draw1);
			if (InputManager::IsPressed(ENTER))
			{
				SceneManager::CloseScene(this);
				return;
			}

		}

		if ((InputManager::GetMousePos().x > 300) && (InputManager::GetMousePos().x < 300 + 10) &&
			(InputManager::GetMousePos().y > 500) && (InputManager::GetMousePos().y < 500 + 10) &&
			(InputManager::IsPressed(Mouse_Left)))
		{
			check = 1;
		}
	}

	void Destroy() override
	{

	}
};


class CellScene : public Scene {
public:
	void Init() override {
		std::srand(time(NULL));
		grid_.sun_per_opt = sun_per;
		grid_.min_per_opt = min_per;
		load_sprites();
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

		DrawData for_draw1(sprites_info[0].spriteID, sprites_info[0].spriteLR, sprites_info[0].sprite_pos);
		for_draw1.size.x = 300;
		for_draw1.size.y = 300;
		GraphicManager::Draw(for_draw1);

		DrawData for_draw2(sprites_info[1].spriteID, sprites_info[1].spriteLR, sprites_info[1].sprite_pos);
		for_draw2.size.x = 100;
		for_draw2.size.y = 100;
		GraphicManager::Draw(for_draw2);

		if ((InputManager::GetMousePos().x > sprites_info[1].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[1].sprite_pos.x + sprites_info[1].size.x) &&
			(InputManager::GetMousePos().y > sprites_info[1].sprite_pos.y) && (InputManager::GetMousePos().y < sprites_info[1].sprite_pos.y + sprites_info[1].size.y))
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				easterr = 1;
			}

		if (easterr == 1)
		{
			Debugger::DrawPoint(InputManager::GetMousePos() , 4, 0, Color::Red());
			if (InputManager::GetMousePos().x <= 0)
			{
				Scene* start = new EastScene;
				SceneManager::CreateScene(start);
				SceneManager::CloseScene(this);
				return;
			}
		}



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

		if ((InputManager::IsPressed(ESC)))
		{
			Scene* start = new StartScene;
			SceneManager::CreateScene(start);
			SceneManager::CloseScene(this);
			return;
		}
	}

	void Destroy() override {
	}

private:
	CellGrid grid_;
	bool is_paused = false;
	int easterr;

	std::vector<sprite_info> sprites_info;
	void load_sprites()
	{
		int texID_miner = GraphicManager::LoadTexture("./tests/grave.png");
		int texID_sun = GraphicManager::LoadTexture("./tests/solokostil.png");

		Vector2F pos_tx = { 0 , 0 };
		Vector2F size = { 1024 , 1024 };
		Vector2F size_fl = { 300 , 300 };
		Vector2F size_fl_gr = { 100 , 100 };
		Vector2F size_gr = { 25 , 25 };

		GraphicPrefabData sprite1(texID_sun, size, 1, pos_tx);
		int spr_ID = GraphicManager::LoadSprite(sprite1);
		sprites_info.push_back(sprite_info(spr_ID, 10, size_fl));

		GraphicPrefabData sprite2(texID_miner, size_gr, 1, pos_tx);
		spr_ID = GraphicManager::LoadSprite(sprite2);
		sprites_info.push_back(sprite_info(spr_ID, 11, size_fl_gr));

		sprites_info[0].sprite_pos.x = 900;
		sprites_info[0].sprite_pos.y = 110;

		sprites_info[1].sprite_pos.x = 1000;
		sprites_info[1].sprite_pos.y = 30;
	}


};

/*std::vector<sprite_info> load()
{

}*/

enum spr_opt
{
	SUN_ , MINER_ , MINUS_ , PLUS_
};

class OptScene : public Scene
{
private:
	std::vector<sprite_info> sprites_info;
	void load_sprites()
	{
		int texID_miner = GraphicManager::LoadTexture("./tests/minerals.png");
		int texID_sun = GraphicManager::LoadTexture("./tests/sun.png");
		int texID_plus = GraphicManager::LoadTexture("./tests/trueplus.png");
		int texID_minus = GraphicManager::LoadTexture("./tests/trueminus.png");

		Vector2F pos_tx = { 0 , 0 };
		Vector2F size = { 1024 , 1024 };
		Vector2F size_main = { 200 , 200 };
		Vector2F size_op = { 100 , 100 };
		
		GraphicPrefabData sprite1(texID_sun, size, 1, pos_tx);
		int spr_ID = GraphicManager::LoadSprite(sprite1);
		sprites_info.push_back(sprite_info(spr_ID, 10, size_main));

		GraphicPrefabData sprite2(texID_miner, size, 1, pos_tx);
		spr_ID = GraphicManager::LoadSprite(sprite2);
		sprites_info.push_back(sprite_info(spr_ID, 10, size_main));

		GraphicPrefabData sprite3(texID_minus, size, 1, pos_tx);
		spr_ID = GraphicManager::LoadSprite(sprite3);
		sprites_info.push_back(sprite_info(spr_ID, 10, size_op));

		GraphicPrefabData sprite4(texID_plus, size, 1, pos_tx);
		spr_ID = GraphicManager::LoadSprite(sprite4);
		sprites_info.push_back(sprite_info(spr_ID, 10, size_op));

		sprites_info[SUN_].sprite_pos.x = 500;
		sprites_info[SUN_].sprite_pos.y = 10;

		sprites_info[MINER_].sprite_pos.x = 500;
		sprites_info[MINER_].sprite_pos.y = 230;

		sprites_info[MINUS_].sprite_pos.x = 750;
		sprites_info[MINUS_].sprite_pos.y = 280;

		sprites_info[PLUS_].sprite_pos.x = 350;
		sprites_info[PLUS_].sprite_pos.y = 280;
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
		DrawData for_draw1(sprites_info[SUN_].spriteID, sprites_info[SUN_].spriteLR, sprites_info[SUN_].sprite_pos);
		for_draw1.size.x = 200;
		for_draw1.size.y = 200;
		GraphicManager::Draw(for_draw1);

		DrawData for_draw2(sprites_info[MINER_].spriteID, sprites_info[MINER_].spriteLR, sprites_info[MINER_].sprite_pos);
		for_draw2.size.x = 200;
		for_draw2.size.y = 200;
		GraphicManager::Draw(for_draw2);

		DrawData for_draw3(sprites_info[MINUS_].spriteID, sprites_info[MINUS_].spriteLR, sprites_info[MINUS_].sprite_pos);
		for_draw3.size.x = 100;
		for_draw3.size.y = 100;
		GraphicManager::Draw(for_draw3);

		DrawData for_draw4(sprites_info[PLUS_].spriteID, sprites_info[PLUS_].spriteLR, sprites_info[PLUS_].sprite_pos);
		for_draw4.size.x = 100;
		for_draw4.size.y = 100;
		GraphicManager::Draw(for_draw4);

		for_draw3.position.y = 60;
		GraphicManager::Draw(for_draw3);

		for_draw4.position.y = 60;
		GraphicManager::Draw(for_draw4);

		int sun_op_y = 60;
		int min_op_y = 280;

		if ((InputManager::GetMousePos().x > sprites_info[PLUS_].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[PLUS_].sprite_pos.x + sprites_info[PLUS_].size.x) &&
			(InputManager::GetMousePos().y > min_op_y) && (InputManager::GetMousePos().y < min_op_y + sprites_info[PLUS_].size.y))
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				min_per += 1;
				std::cout << min_per << std::endl;
			}

		if ((InputManager::GetMousePos().x > sprites_info[MINUS_].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[MINUS_].sprite_pos.x + sprites_info[MINUS_].size.x) &&
			(InputManager::GetMousePos().y > min_op_y) && (InputManager::GetMousePos().y < min_op_y + sprites_info[MINUS_].size.y))
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				min_per -= 1;
				std::cout << min_per << std::endl;
			}

		if ((InputManager::GetMousePos().x > sprites_info[PLUS_].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[PLUS_].sprite_pos.x + sprites_info[PLUS_].size.x) &&
			(InputManager::GetMousePos().y > sun_op_y) && (InputManager::GetMousePos().y < sun_op_y + sprites_info[PLUS_].size.y))
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				sun_per += 1;
				std::cout << sun_per << std::endl;
			}

		if ((InputManager::GetMousePos().x > sprites_info[MINUS_].sprite_pos.x) && (InputManager::GetMousePos().x < sprites_info[MINUS_].sprite_pos.x + sprites_info[MINUS_].size.x) &&
			(InputManager::GetMousePos().y > sun_op_y) && (InputManager::GetMousePos().y < sun_op_y + sprites_info[MINUS_].size.y))
			if ((InputManager::IsPressed(Mouse_Left)))
			{
				sun_per -= 1;
				std::cout << sun_per << std::endl;
			}

		if ((InputManager::IsPressed(ESC)))
		{
			Scene* start = new StartScene;
			SceneManager::CreateScene(start);
			SceneManager::CloseScene(this);
			return;
		}
	}

	void Destroy() override
	{

	}


};

enum buttons_main
{
	START , START_PRESSED , OPT , OPT_PRESSED , BACK_ , BACK_PRESSED , FON
};

void StartScene::load_sprites()
{
	int texID = GraphicManager::LoadTexture("./tests/fullbuttonsprite.png");
	int texID_fon = GraphicManager::LoadTexture("./tests/kostili.png");
	Vector2F size = { 731 , 256 };
	Vector2F size_f = { 1024 , 1024 };
	Vector2F fon = { 450 , 600 };
	Vector2F pos_fon = { 0 , 0 };
	Vector2F size_spr = { 600 , 200 };
	Vector2F pos_tx = { 0 , 0 };
	Vector2F pos_fl = { 10 , 10 };
	for (int i = 0; i < 6; ++i)
	{
		GraphicPrefabData sprite(texID, size, 1, pos_tx);
		int spr_ID = GraphicManager::LoadSprite(sprite);
		sprites_info.push_back(sprite_info(spr_ID , 10 , size_spr));
		pos_tx.x += 731;
	}
	GraphicPrefabData sprite(texID_fon, size_f, 1, pos_fon);
	int spr_ID = GraphicManager::LoadSprite(sprite);
	sprites_info.push_back(sprite_info(spr_ID, 1, fon));
	sprites_info[FON].sprite_pos.x = 450;
	sprites_info[FON].sprite_pos.y = 0;

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
	
void StartScene::Init()
{
	std::srand(time(NULL));
	GraphicManager::ShowFPS(NULL);

	load_sprites();
}

void StartScene::Update()
{

	DrawData for_draw(sprites_info[FON].spriteID, sprites_info[FON].spriteLR, sprites_info[FON].sprite_pos);
	for_draw.size.x = 900;
	for_draw.size.y = 600;
	GraphicManager::Draw(for_draw);

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
			Scene* start = new OptScene;
			SceneManager::CreateScene(start);
			SceneManager::CloseScene(this);
			return;
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

void StartScene::Destroy()
{

}
