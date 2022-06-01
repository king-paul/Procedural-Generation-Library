#include <iostream>
#include <wtypes.h>
#include <comutil.h>

#include "DungeonGenerator.h"
#include "CaveGenerator.h"
#include "MeshGenerator.h"

using namespace std;

void GenerateDungeon()
{
	int width = 70;
	int height = 70;
	Coord startPos = { width / 2, height / 2 };
	ProceduralGeneration::RandomWalkParameters randomWalk(15, 50, false);

	ProceduralGeneration::DungeonGenerator* dungeon =
		//new ProceduralGeneration::RandomWalkGenerator(width, height, startPos, 50, 15, false);
		//new ProceduralGeneration::CorridorFirstGenerator(width, height, startPos, 30, 10, 0.5f,
			//{ 15, 50, false });
		new ProceduralGeneration::RoomFirstGenerator(width, height, { 0, 0 }, 10, 10, 3, true, randomWalk);

	dungeon->Generate();
	dungeon->DrawDungeon();

	delete dungeon;
}

void GenerateCave()
{
	ProceduralGeneration::CaveGenerator* caveGenerator = new ProceduralGeneration::CaveGenerator();
	///caveGenerator.PrintMapToConsole();
	caveGenerator->GenerateMap();
	caveGenerator->PrintMapToConsole();
	//caveGenerator->GetSquareGrid()->PrintConfigurations();


	//ProceduralGeneration::MeshGenerator* meshGenerator = new ProceduralGeneration::MeshGenerator();
	//meshGenerator->GenerateMesh(caveGenerator->GetMap(), 1, 5);
	
	//vector<int>* baseTriangles = meshGenerator->GetBaseTriangles();
	//vector<Vector3>* baseVertices = meshGenerator->GetBaseVertices();
	//meshGenerator->GetWallTriangles();
	//meshGenerator->GetWallVertices();

	/*
	cout << "Printing triangle data:" << endl;
	for (int triangle : *baseTriangles)
	{
		cout << triangle << ", ";
	}
	cout << endl;*/

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

	//GenerateDungeon();
	GenerateCave();

	return 0;
}