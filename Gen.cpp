#include "./CellsSource/Gen.h"

direction TransformWithRotation(direction dir, direction rot) {
	switch (dir)
	{
	case direction::UP:
		switch (rot)
		{
		case direction::UP: return direction::UP;
		case direction::DOWN: return direction::DOWN;
		case direction::LEFT: return direction::LEFT;
		case direction::RIGHT: return direction::RIGHT;
		default: return direction::UP;
		}
		break;
	case direction::DOWN:
		switch (rot)
		{
		case direction::UP: return direction::DOWN;
		case direction::DOWN: return direction::UP;
		case direction::LEFT: return direction::RIGHT;
		case direction::RIGHT: return direction::LEFT;
		default: return direction::UP;
		}
		break;
	case direction::LEFT:
		switch (rot)
		{
		case direction::UP: return direction::LEFT;
		case direction::DOWN: return direction::RIGHT;
		case direction::LEFT: return direction::DOWN;
		case direction::RIGHT: return direction::UP;
		default: return direction::UP;
		}
		break;
	case direction::RIGHT:
		switch (rot)
		{
		case direction::UP: return direction::RIGHT;
		case direction::DOWN: return direction::LEFT;
		case direction::LEFT: return direction::UP;
		case direction::RIGHT: return direction::DOWN;
		default: return direction::UP;
		}
		break;
	default:
		return direction::UP;
		break;
	}
}