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
	int wallThresholdSize, int roomThresholdSize, int passageWidth, bool useRandomSeed, int seed,
	bool generateMesh, float squareSize, float wallheight)
{
	CaveGenerator* cave = new CaveGenerator(width, height, fillPercent, smoothingIterations, borderSize,
		wallThresholdSize, roomThresholdSize, passageWidth, useRandomSeed, seed,
		generateMesh, squareSize, wallheight);

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
int GetSeedValue(CaveGenerator* cave)
{
	return cave->GetSeed();
}


extern "C" __declspec(dllexport)
MeshGenerator* GenerateMesh(CaveGenerator* cave, float tileSize, float wallHeight)
{
	MeshGenerator* mesh = new MeshGenerator(tileSize, wallHeight);
	mesh->GenerateMesh(cave->GetMap());

	return mesh;
}

extern "C" __declspec(dllexport)
int* GetBaseTriangles(CaveGenerator* cave)
{
	std::vector<int> trianglesVector = *(cave->GetMesh()->GetBaseTriangles());
	int* triangles = new int[trianglesVector.size()];

	for (int i = 0; i < trianglesVector.size(); i++)
	{
		triangles[i] = trianglesVector[i];
	}

	return triangles;
}

extern "C" __declspec(dllexport)
float* GetBaseVerticies(CaveGenerator* cave, char component)
{
	std::vector<Vector3>* vertexVector = cave->GetMesh()->GetBaseVertices();
	float* vertices = new float[vertexVector->size()];

	for (int i = 0; i < vertexVector->size(); i++)
	{
		switch (component)
		{
			case 'x': case 'X': vertices[i] = (*vertexVector)[i].x;
				break;
			case 'y': case 'Y': vertices[i] = (*vertexVector)[i].y;
				break;
			case 'z': case 'Z': vertices[i] = (*vertexVector)[i].z;
				break;

			default:
				return vertices;
		}
	}

	return vertices;
}

extern "C" __declspec(dllexport)
int GetTotalBaseTriangles(CaveGenerator* cave)
{
	return (int) cave->GetMesh()->GetBaseTriangles()->size();
}

extern "C" __declspec(dllexport)
int GetTotalWallTriangles(CaveGenerator * cave)
{
	return (int) cave->GetMesh()->GetWallTriangles()->size();
}

extern "C" __declspec(dllexport)
int GetTotalBaseVertices(CaveGenerator * cave)
{
	return (int)cave->GetMesh()->GetBaseVertices()->size();
}

extern "C" __declspec(dllexport)
int GetTotalWallVertices(CaveGenerator * cave)
{
	return (int)cave->GetMesh()->GetWallVertices()->size();
}

extern "C" __declspec(dllexport)
int* GetWallTriangles(CaveGenerator * cave)
{
	std::vector<int> trianglesVector = *(cave->GetMesh()->GetWallTriangles());
	int* triangles = new int[trianglesVector.size()];

	for (int i = 0; i < trianglesVector.size(); i++)
	{
		triangles[i] = trianglesVector[i];
	}

	return triangles;
}

extern "C" __declspec(dllexport)
float* GetWallVerticies(CaveGenerator * cave, char component)
{
	std::vector<Vector3>* vertexVector = cave->GetMesh()->GetWallVertices();
	float* vertices = new float[vertexVector->size()];

	for (int i = 0; i < vertexVector->size(); i++)
	{
		switch (component)
		{
		case 'x': case 'X': vertices[i] = (*vertexVector)[i].x;
			break;
		case 'y': case 'Y': vertices[i] = (*vertexVector)[i].y;
			break;
		case 'z': case 'Z': vertices[i] = (*vertexVector)[i].z;
			break;

		default:
			return vertices;
		}
	}

	return vertices;
}

extern "C" __declspec(dllexport)
void DeleteCavePointer(CaveGenerator* cave)
{
	delete cave;
}