#include <iostream>
#include <wtypes.h>
#include <comutil.h>

#include "DungeonGenerator.h"

extern "C" __declspec(dllexport) int AddNumbers(int a, int b);

int AddNumbers(int a, int b) {
	return a + b;
}

int main()
{
	srand(time(NULL));
	int size = 50;

	ProceduralGeneration::DungeonGenerator* dungeon = 
		new ProceduralGeneration::RandomWalkDungeonGenerator(size, { 25, 25 }, 50, 15, false);

	dungeon->Generate();
	dungeon->DrawMap();

	//delete &dungeon;

	//std::cout << "Hello World" << std::endl;
	return 0;
}