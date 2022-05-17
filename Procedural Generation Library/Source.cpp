#include <iostream>
#include <wtypes.h>
#include <comutil.h>

#include "DungeonGenerator.h"

extern "C"
{
	__declspec(dllexport)
	void TestFunction()
	{
		std::cout << "Test function is working." << std::endl;
	}
}


int main()
{
	srand((unsigned int) time(NULL));
	int size = 50;

	ProceduralGeneration::DungeonGenerator* dungeon = 
		new ProceduralGeneration::RandomWalkDungeonGenerator(size, { 25, 25 }, 50, 15, false);

	dungeon->Generate();
	dungeon->DrawMap();

	//delete &dungeon;

	//std::cout << "Hello World" << std::endl;
	return 0;
}