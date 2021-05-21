#pragma once

const size_t BEH_DNA_SIZE = 64;
const size_t PROP_DNA_SIZE = 64;

enum class cell_type_t {
	ERROR,
	ALIVE,
	CORPSE,
	WALL
};

struct alive_data_t {
	Vector2I grid_pos;
	float energy;
	bool to_delete;
	Color color;
	Color family_color;
	bool is_eatable;

	struct efficenty_t {
		float sun_eff;
		float minerals_eff;
		float predator_eff;
		float corpse_eff;
	} efficenty;

	int mutation_chance;

	size_t current_beh_gen;
	Gen Beh_DNA[BEH_DNA_SIZE];
	property_gen_t Prop_DNA[PROP_DNA_SIZE];
	direction rotation;

	bool is_rage;
	bool is_divided;
	bool is_died;
	int age;
	float energy_per_step;
	float max_energy;
	int actions_per_step;
	direction cur_dir;

	int corpse_timer;
};

struct corpse_data_t {
	int corpse_timer;

	Vector2I grid_pos;
	float energy;
};

struct wall_data_t {
	Vector2I grid_pos;
};

struct all_cell_data_t {
	alive_data_t as_alive;
	corpse_data_t as_corpse;
	wall_data_t as_wall;
};

struct cell_data_t {
	cell_type_t type;
	all_cell_data_t data;
};