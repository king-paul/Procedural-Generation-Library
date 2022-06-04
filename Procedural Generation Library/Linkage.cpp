#include "DungeonGenerator.h"
#include "CaveGenerator.h"
#include "MeshGenerator.h"
#include <wtypes.h>

using namespace ProceduralGeneration;

/*********************
 * Dungeon Generator *
 *********************/
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
	return (int) dungeonPtr->GetSpaceValue(x, y);
}

extern "C" __declspec(dllexport)
DungeonGenerator* CreateCorridorFirstDungeon(int width, int height, int startX, int staryY, int corridorLength, int totalCorridors, float roomPercent,
											   int roomWalkIterations, int roomWalkLength, bool startRandomlyEachWalk)
{
	return new ProceduralGeneration::CorridorFirstGenerator(width, height, {startX, staryY}, corridorLength, totalCorridors, roomPercent,
														   {roomWalkIterations, roomWalkLength, startRandomlyEachWalk});
}

extern "C" __declspec(dllexport)
DungeonGenerator * CreateRoomFirstDungeon(int width, int height, int startX, int staryY, int minRoomWidth, int minRoomHeight, int offset, bool randomWalkRooms,
										  int roomWalkIterations, int roomWalkLength, bool startRandomlyEachWalk)
{
	return new ProceduralGeneration::RoomFirstGenerator(width, height, { startX, staryY }, minRoomWidth, minRoomHeight, offset, randomWalkRooms,
													   {roomWalkIterations, roomWalkLength, startRandomlyEachWalk});
}

extern "C" __declspec(dllexport)
void GenerateDungeon(DungeonGenerator* dungeonPtr)
{
	dungeonPtr->Generate();
}

/******************
 * Cave Generator *
 ******************/
extern "C" __declspec(dllexport)
CaveGenerator* GenerateCave(int width, int height, int fillPercent, int smoothingIterations, int borderSize,
	int wallThresholdSize, int roomThresholdSize, int passageWidth, bool forceAccessToMain, bool useRandomSeed, int seed)
{
	CaveGenerator* cave = new CaveGenerator(width, height, fillPercent, smoothingIterations, borderSize, 
		wallThresholdSize, roomThresholdSize, passageWidth, forceAccessToMain, useRandomSeed, seed);

	cave->GenerateMap();
	return cave;
}

extern "C" __declspec(dllexport)
bool IsWall(int x, int y, CaveGenerator* cave)
{
	auto map = cave->GetBorderedMap();
	
	if (map->get(x, y) == 1)
		return true;
	else
		return false;
}

extern "C" __declspec(dllexport)
int GetMarchingSquareValue(int x, int y, CaveGenerator * cave)
{
	return cave->GetSquareGrid()->GetSquares()->at(x, y).configuration;
}

extern "C" __declspec(dllexport)
MeshGenerator* GenerateMesh(CaveGenerator* cave, float gridSize, float wallHeight)
{
	MeshGenerator* mesh = new MeshGenerator();
	mesh->GenerateMesh(cave->GetMap(), gridSize, wallHeight);

	return mesh;
}

int GetBaseTriangle(MeshGenerator* mesh, int index)
{
	return (*mesh->GetBaseTriangles())[index];
}

float GetBaseVertexX(MeshGenerator* mesh, int index)
{
	return (*mesh->GetBaseVertices())[index].x;
}

float GetBaseVertexY(MeshGenerator* mesh, int index)
{
	return (*mesh->GetBaseVertices())[index].y;
}

float GetBaseVertexZ(MeshGenerator* mesh, int index)
{
	return (*mesh->GetBaseVertices())[index].z;
}

int GetWallTriangle(MeshGenerator* mesh, int index)
{
	return (*mesh->GetWallTriangles())[index];
}

float GetWallVertexX(MeshGenerator* mesh, int index)
{
	return (*mesh->GetWallVertices())[index].x;
}

float GetWallVertexY(MeshGenerator* mesh, int index)
{
	return (*mesh->GetWallVertices())[index].y;
}

float GetWallVertexZ(MeshGenerator* mesh, int index)
{
	return (*mesh->GetWallVertices())[index].z;
}