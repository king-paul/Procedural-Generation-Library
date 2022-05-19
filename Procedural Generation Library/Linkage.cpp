#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

extern "C" __declspec(dllexport)
DungeonGenerator* CreateRandomWalkRoom(int width, int height, int startX, int startY, int iterations, int walkLength, bool startRandomly)
{
	srand((unsigned int) time(NULL));
	DungeonGenerator* dungeon = new RandomWalkGenerator(width, height, {startX, startY}, iterations, walkLength, startRandomly);
	dungeon->Generate();

	return dungeon;
}

extern "C" __declspec(dllexport)
int GetSpaceValue(DungeonGenerator* dungeon, int x, int y)
{
	return dungeon->GetSpaceValue(x, y);
}