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

	int width = 70;
	int height = 70;
	Coord startPos = { width/2, height/2 };
	ProceduralGeneration::RandomWalkParameters randomWalk(15, 50, false);

	ProceduralGeneration::DungeonGenerator* dungeon =
		//new ProceduralGeneration::RandomWalkGenerator(width, height, startPos, 50, 15, false);
		//new ProceduralGeneration::CorridorFirstGenerator(width, height, startPos, 30, 10, 0.5f,
			//{ 15, 50, false });
		new ProceduralGeneration::RoomFirstGenerator(width, height, { 0, 0 }, 10, 10, 3, true, randomWalk);

	dungeon->Generate();
	dungeon->DrawDungeon();

	delete dungeon;

	return 0;
}