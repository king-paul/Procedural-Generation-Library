#include "algorithms.h"
#include <iostream>

using namespace ProceduralGeneration;

CoordList Algorithms::GetWalls(CoordList floorPositions, Coord boundary)
{
	CoordList walls = CoordList();

	// calculcate positions for top dottom left and right floor edges
	var sideWallPositions = FindWallsInDirection(floorPositions, Direction2D::CardinalDirections(), boundary);
	// calcualate positions for floor corners
	var cornerWallPositions = FindWallsInDirection(floorPositions, Direction2D::DiagonalDirections(), boundary);

	// join the two vectors together
	walls.insert(walls.begin(), sideWallPositions.begin(), sideWallPositions.end());
	walls.insert(walls.begin(), cornerWallPositions.begin(), cornerWallPositions.end());

	return walls;
}

CoordList Algorithms::FindWallsInDirection(CoordList floorPositions, CoordList directionList, Coord boundary)
{
	CoordList wallPositions;
	
	for(var position : floorPositions)
	{
		for(var direction : directionList)
		{
			var neighbourPosition = position + direction; // gets the next direction

			// if out of bounds of the map don't create the wall
			if(isOutOfBounds(boundary.x, boundary.y, neighbourPosition))
				continue;

			// if the neighbour position is not in the floor positions list
			// add it to the wall positions
			if (!CoordInList(&floorPositions, neighbourPosition))
				wallPositions.Add(neighbourPosition);
		}
	}

	return wallPositions;
}