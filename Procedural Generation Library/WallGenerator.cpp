#include "algorithms.h"
#include <iostream>

using namespace ProceduralGeneration;

CoordList Algorithms::GetWalls(CoordList floorPositions)
{
	CoordList walls = CoordList();

	// calculcate positions for top dottom left and right floor edges
	var sideWallPositions = FindWallsInDirection(floorPositions, Direction2D::CardinalDirections());
	// calcualate positions for floor corners
	var cornerWallPositions = FindWallsInDirection(floorPositions, Direction2D::DiagonalDirections());

	// create the walls from the calculated positions
	//var sideWalls = GetBasicWalls(basicWallPositions, floorPositions);
	//var cornerWalls = GetCornerWalls(cornerWallPositions, floorPositions);

	// join the two vectors together
	walls.insert(walls.begin(), sideWallPositions.begin(), sideWallPositions.end());
	walls.insert(walls.begin(), cornerWallPositions.begin(), cornerWallPositions.end());

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

			// if the neighbour position is not in the floor positions list, add it
			if (std::find(floorPositions.begin(), floorPositions.end(), neighbourPosition) == floorPositions.end())
				wallPositions.Add(neighbourPosition);
		}
	}

	return wallPositions;
}