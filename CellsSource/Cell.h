#pragma once
#include "BasicCell.h"
#include <iostream>

struct parent_color_t {
	Color family_color;
	Color beh_color;
};

class Cell : public BasicCell
{
public:
	Cell(Vector2I start_pos, float start_energy);
	Cell(Vector2I start_pos, float start_energy, const std::array<Gen, BEH_DNA_SIZE>& parent_DNA, const std::array<property_gen_t, PROP_DNA_SIZE>& parent_prop_DNA, parent_color_t parent_color = { Color::Green(), Color::Green() });


	void Update() override;
	void Draw() override;
	void Destroy() override;

	void PrintDNA() {
		for (int i = 0; i < BEH_DNA_SIZE; i++) {
			std::cout << i + 1 << ": ";
			switch (Beh_DNA_[i].ID)
			{
			case gen_order::EAT_SUN:               std::cout << "EAT_SUN               "; break;
			case gen_order::DO_NOTHING:            std::cout << "DO_NOTHING            "; break;
			case gen_order::EAT_MINERALS:          std::cout << "EAT_MINERALS          "; break;
			case gen_order::GO:                    std::cout << "GO                    "; break;
			case gen_order::JUMP:                  std::cout << "JUMP                  "; break;
			case gen_order::JUMP_IF_CELL_NEAR:     std::cout << "JUMP_IF_CELL_NEAR     "; break;
			case gen_order::JUMP_IF_CORPSE_NEAR:   std::cout << "JUMP_IF_CORPSE_NEAR   "; break;
			case gen_order::JUMP_IF_ALIVE_NEAR:    std::cout << "JUMP_IF_ALIVE_NEAR    "; break;
			case gen_order::JUMP_IF_RELATIVE_NEAR: std::cout << "JUMP_IF_RELATIVE_NEAR "; break;
			case gen_order::BECOME_RAGE:           std::cout << "BECOME_RAGE           "; break;
			case gen_order::ROTATE:                std::cout << "ROTATE                "; break;
			case gen_order::JUMP_IF_ENERGY_HIGHER: std::cout << "JUMP_IF_ENERGY_HIGHER "; break;
			case gen_order::JUMP_IF_ENERGY_LESS:   std::cout << "JUMP_IF_ENERGY_LESS   "; break;
			case gen_order::JUMP_IF_Y_HIGHER:      std::cout << "JUMP_IF_Y_HIGHER      "; break;
			case gen_order::JUMP_IF_Y_LESS:        std::cout << "JUMP_IF_Y_LESS        "; break;
			case gen_order::DIVIDE:                std::cout << "DIVIDE                "; break;
			case gen_order::EAT_CORPSE:            std::cout << "EAT_CORPSE            "; break;

			default:
				break;
			}

			switch (Beh_DNA_[i].dir)
			{
			case direction::UP:        std::cout << "UP    "; break;
			case direction::DOWN:      std::cout << "DOWN  "; break;
			case direction::LEFT:      std::cout << "LEFT  "; break;
			case direction::RIGHT:     std::cout << "RIGHT "; break;
			default:
				break;
			}

			std::cout << +static_cast<unsigned char>(Beh_DNA_[i].data) % 64 + 1 << " ";
			std::cout << +static_cast<unsigned char>(Beh_DNA_[i].additional_data) + 1 << "\n";
		}
	}

private:

	struct efficenty_t {
		float sun_eff = 1.f;
		float minerals_eff = 1.f;
		float predator_eff = 1.f;
		float corpse_eff = 1.f;
	} efficenty_;

	int mutation_chance = 1;

	size_t current_beh_gen = 0;
	std::array<Gen, BEH_DNA_SIZE> Beh_DNA_;
	std::array<property_gen_t, PROP_DNA_SIZE> Prop_DNA_;
	direction rotation_ = direction::UP;

	bool is_rage_ = false;
	bool is_divided_ = false;
	bool is_died_ = false;
	int age_ = 0;
	float energy_per_step = 1.f;
	float max_energy = 100.f;
	int actions_per_step = 5;
	direction cur_dir = direction::UP;
	void BornCell();
};