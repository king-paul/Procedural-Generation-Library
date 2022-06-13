#include <iostream>
#include <wtypes.h>
#include <comutil.h>

#include "DungeonGenerator.h"
#include "CaveGenerator.h"
#include "MeshGenerator.h"

using namespace std;

void GenerateDungeon()
{
	int width = 100;
	int height = 100;
	int minRoomWidth = 10;
	int minRoomHeight = 10;
	int roomOffset = 3;
	bool randomWalkRooms = false;

	int iterations = 50;
	int walkLength = 15;
	bool startRandomly = false;

	Coord startPos = { 0, 0 };
	//{ width / 2, height / 2 };
	ProceduralGeneration::RandomWalkParameters randomWalk(iterations, walkLength, startRandomly);

	ProceduralGeneration::DungeonGenerator* dungeon =
		//new ProceduralGeneration::RandomWalkGenerator(width, height, startPos, 50, 15, false);
		//new ProceduralGeneration::CorridorFirstGenerator(width, height, startPos, 30, 10, 0.5f,
			//{ 15, 50, false });
		new ProceduralGeneration::RoomFirstGenerator(width, height, startPos, minRoomWidth, minRoomHeight, roomOffset,
													 randomWalkRooms, randomWalk);

	dungeon->Generate();
	dungeon->DrawDungeon();

	delete dungeon;
}

void GenerateCave()
{
	int width = 128;
	int height = 72;
	int smoothingIterations = 5;
	int borderSize = 1;
	int fillPercent = 50;
	int wallThresholdSize = 50;
	int roomThresholdSize = 50;
	int passageWidth = 1;
	bool forceAccessToMain = false;
	bool useRandomSeed = false;
	int seed = 0;
	bool generateMesh = true;
	float squareSize = 1;
	float wallHeight = 5;

	ProceduralGeneration::CaveGenerator* caveGenerator = new ProceduralGeneration::CaveGenerator(
		width, height, fillPercent, smoothingIterations, borderSize, wallThresholdSize, roomThresholdSize, passageWidth, forceAccessToMain,
		useRandomSeed, seed, generateMesh, squareSize, wallHeight);

	//for (int i = 0; i < 10; i++)
	//{
		caveGenerator->GenerateMap();
		//caveGenerator->PrintCave();
		//system("Pause");
		
	//}

	//caveGenerator->PrintCaveWithGrid();
	//caveGenerator->GetSquareGrid()->PrintConfigurations();
	//caveGenerator->GetSquareGrid()->PrintConfigurationsWithGrid();
	
	/*
	ProceduralGeneration::MeshGenerator* meshGenerator = caveGenerator->GetMesh();
	vector<int>* baseTriangles = meshGenerator->GetBaseTriangles();
	vector<int>* wallTriangles = meshGenerator->GetWallTriangles();

	//vector<Vector3>* baseVertices = meshGenerator->GetBaseVertices();
	//meshGenerator->GetWallTriangles();
	//meshGenerator->GetWallVertices();
	
	cout << "Printing triangle data:" << endl;
	for (int triangle : *wallTriangles)
	{
		cout << triangle << ", ";
	}
	cout << endl;

	/*
	cout << "Printing vertex data:" << endl;
	for (Vector3 point : *baseVertices)
	{
		cout << point.x << ", " << point.y << ", " << point.z << endl;
	}*/

	delete caveGenerator;
	//delete meshGenerator;
}

int main()
{
	//srand((unsigned int) time(NULL));

	GenerateDungeon();
	//GenerateCave();

	return 0;
}