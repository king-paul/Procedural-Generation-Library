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
	ProceduralGeneration::RandomWalkParameters(15, 50, false);

	ProceduralGeneration::DungeonGenerator* dungeon =
		//new ProceduralGeneration::RandomWalkGenerator(width, height, startPos, 50, 15, false);
		new ProceduralGeneration::CorridorFirstGenerator(width, height, startPos, 30, 10, 0.5f,
			{ 15, 50, false });

	dungeon->Generate();
	dungeon->DrawDungeon();

	delete dungeon;

	return 0;
}