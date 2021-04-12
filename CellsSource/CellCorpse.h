#pragma once
#include "BasicCell.h"
class CellCorpse : public BasicCell
{
public:
	CellCorpse(Vector2I start_pos, float start_energy);

	void Update() override;
	void Draw() override;
	void Destroy() override;

private:
	int corpse_timer = 0;
	const int max_time = 500;
};