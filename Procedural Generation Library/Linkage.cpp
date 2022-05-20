#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

extern "C" __declspec(dllexport)
DungeonGenerator* CreateRandomWalkRoom(int width, int height, int startX, int startY, int iterations, int walkLength, bool startRandomly)
{
	srand((unsigned int) time(NULL));
	DungeonGenerator* dungeon = new RandomWalkGenerator(width, height, {startX, startY}, iterations, walkLength, startRandomly);

	return dungeon;
}

extern "C" __declspec(dllexport)
int GetSpaceValue(DungeonGenerator* dungeonPtr, int x, int y)
{
	return dungeonPtr->GetSpaceValue(x, y);
}

extern "C" __declspec(dllexport)
DungeonGenerator* CreateCorridorFirstDungeon(int width, int height, int startX, int staryY, int corridorLength, int totalCorridors, float roomPercent,
											   int roomWalkIterations, int roomWalkLength, bool startRandomlyEachWalk)
{
	return new ProceduralGeneration::CorridorFirstGenerator(width, height, {startX, staryY}, corridorLength, totalCorridors, roomPercent,
														   {roomWalkIterations, roomWalkLength, startRandomlyEachWalk});
}

extern "C" __declspec(dllexport)
DungeonGenerator * CreateRoomFirstDungeon(int width, int height, int startX, int staryY, int minRoomWidth, int minRoomHeight, bool randomWalkRooms,
											int roomWalkIterations = 15, int roomWalkLength = 10, bool startRandomlyEachWalk = false)
{
	return new ProceduralGeneration::RoomFirstGenerator(minRoomWidth, minRoomHeight, width, height, { startX, staryY }, randomWalkRooms,
													   {roomWalkIterations, roomWalkLength, startRandomlyEachWalk});
}

extern "C" __declspec(dllexport)
void GenerateDungeon(DungeonGenerator* dungeonPtr)
{
	dungeonPtr->Generate();
}