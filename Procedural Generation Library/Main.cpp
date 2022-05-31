#include <iostream>
#include <wtypes.h>
#include <comutil.h>

#include "DungeonGenerator.h"
#include "CaveGenerator.h"
#include "MeshGenerator.h"

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
	CaveGenerator caveGenerator;// = new CaveGenerator();
	caveGenerator.GenerateMap();

	ProceduralGeneration::MeshGenerator meshGenerator;
	//meshGenerator.GenerateMesh();

	//delete caveGenerator;
}

int main()
{
	//srand((unsigned int) time(NULL));

	//GenerateDungeon();
	GenerateCave();

	return 0;
}