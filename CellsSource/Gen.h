#pragma once

enum class gen_order {
	DO_NOTHING,
	GO,
	EAT_SUN,
	EAT_MINERALS,
	BECOME_RAGE,
	ROTATE,
	JUMP,
	JUMP_IF_CELL_NEAR,
	JUMP_IF_ALIVE_NEAR,
	JUMP_IF_CORPSE_NEAR,
	JUMP_IF_RELATIVE_NEAR,
	JUMP_IF_ENERGY_LESS,
	JUMP_IF_ENERGY_HIGHER,
	JUMP_IF_Y_HIGHER,
	JUMP_IF_Y_LESS,
	EAT_CORPSE,
	DIVIDE,

	gen_order_count
};

enum class direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,

	direction_count
};

struct Gen
{
	gen_order ID;
	direction dir;
	char data, additional_data;
};

direction TransformWithRotation(direction dir, direction rot);