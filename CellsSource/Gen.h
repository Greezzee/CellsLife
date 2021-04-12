#pragma once

enum class gen_order {
	DO_NOTHING,
	GO,
	EAT_SUN,
	EAT_MINERALS,
	BECOME_RAGE,

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
};