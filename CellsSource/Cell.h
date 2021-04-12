#pragma once
#include "BasicCell.h"
class Cell : public BasicCell
{
public:
	Cell(Vector2I start_pos, float start_energy);
	Cell(Vector2I start_pos, float start_energy, const std::array<Gen, BEH_DNA_SIZE>& parent_DNA);


	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	size_t current_beh_gen = 0;
	std::array<Gen, BEH_DNA_SIZE> Beh_DNA_;

	bool is_rage_ = false;
	void BornCell();
};