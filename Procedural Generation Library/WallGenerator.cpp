#include "algorithms.h"

using namespace ProceduralGeneration;

CoordList Algorithms::GetWalls(CoordList floorPositions)
{
	CoordList walls = CoordList();

	// calculcate positions for top dottom left and right floor edges
	var basicWallPositions = FindWallsInDirection(floorPositions, Direction2D::CardinalDirections());
	// calcualate positions for floor corners
	var cornerWallPositions = FindWallsInDirection(floorPositions, Direction2D::DiagonalDirections());

	// create the walls from the calculated positions
	var sideWalls = GetBasicWalls(basicWallPositions, floorPositions);
	var cornerWalls = GetCornerWalls(cornerWallPositions, floorPositions);

	// join the two vectors together
	walls.insert(walls.begin(), sideWalls.begin(), sideWalls.end());
	walls.insert(walls.begin(), cornerWalls.begin(), cornerWalls.end());

	return walls;
}


void Algorithms::CreateEdgeWalls(CoordList& edgeWallPositions, CoordList floorPositions)
{
	edgeWallPositions = FindWallsInDirection(floorPositions, Direction2D::CardinalDirections());
}

void Algorithms::CreateCornerWalls(CoordList& cornerWallPositions, CoordList floorPositions)
{
	cornerWallPositions = FindWallsInDirection(floorPositions, Direction2D::DiagonalDirections());
}

CoordList Algorithms::GetBasicWalls(CoordList edgeWallPositions, CoordList floorPositions)
{
	CoordList walls;
	
	// iterates through every position to plave a wall
	for (var position : edgeWallPositions)
	{
		// searches each cardinal direction from the current position and if it
	    // does not contain a floor adds it to the list
		for (var direction : Direction2D::CardinalDirections())
		{
			var neighbourPosition = position + direction;

			// checks if the neighbour position is in the floor positions
			if (std::find(floorPositions.begin(), floorPositions.end(), neighbourPosition) == floorPositions.end())
			{
				walls.Add(neighbourPosition);
			}
		}
	}

	return walls;
}

CoordList Algorithms::GetCornerWalls(CoordList cornerWallPositions, CoordList floorPositions)
{
	CoordList walls;

	
	// iterates through every position to plave a wall
	for (var position : cornerWallPositions)
	{
		for (var direction : Direction2D::AllDirections())
		{
			var neighbourPosition = position + direction;

			// checks if the neighbour position is in the floor positions
			if (std::find(floorPositions.begin(), floorPositions.end(), neighbourPosition) == floorPositions.end())
			{
				walls.Add(neighbourPosition);
			}
		}
	}

	return walls;
}

CoordList Algorithms::FindWallsInDirection(CoordList floorPositions, CoordList directionList)
{
	CoordList wallPositions;

	
	for(var position : floorPositions)
	{
		for(var direction : directionList)
		{
			var neighbourPosition = position + direction; // gets the next direction

			//var iterator = std::find(floorPositions.begin(), floorPositions.end(), neighbourPosition);

			// if the neighbour position is no in the floor positions list, add it
			if (std::find(floorPositions.begin(), floorPositions.end(), neighbourPosition) == floorPositions.end())
				wallPositions.Add(neighbourPosition);
		}
	}

	return wallPositions;
}