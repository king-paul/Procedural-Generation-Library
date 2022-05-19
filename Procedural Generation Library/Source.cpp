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
	int width = 200;
	int height = 200;
	Coord startPos = { width/2, height/2 };

	ProceduralGeneration::DungeonGenerator* dungeon =
		//new ProceduralGeneration::RandomWalkGenerator(width, height, startPos, 50, 15, false);
		new ProceduralGeneration::CorridorFirstGenerator(width, height, startPos, 20, 10);

	dungeon->Generate();
	dungeon->DrawMap();

	delete dungeon;

	return 0;
}