//#include <vld.h>
#include <iostream>
#include "../Engine/AllEngine.h"
#include "../CellsSource/CellScene.h"

int main()
{
	GameManager::Init();
	Scene* start = new StartScene;
	start->Init();
	GameManager::Launch(start);
	return 0;
}