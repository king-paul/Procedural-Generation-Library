#include "algorithms.h"
#include "WallTypes.h"

#include <iostream>

using namespace ProceduralGeneration;

vector<Wall> Algorithms::GetWalls(CoordList& floorPositions, Coord boundary)
{
	//CoordList wallPositions = CoordList();

	// calculcate positions for top dottom left and right floor edges
	var sideWallPositions = FindWallsInDirection(floorPositions, Direction2D::CardinalDirections(), boundary);
	// calcualate positions for floor corners
	var cornerWallPositions = FindWallsInDirection(floorPositions, Direction2D::AllDirections(), boundary);

	vector<Wall> walls = GetWallTiles(floorPositions, sideWallPositions, cornerWallPositions);

	// join the two vectors together
	//allPositions.insert(wallPositions.begin(), sideWallPositions.begin(), sideWallPositions.end());
	//wallPositions.insert(wallPositions.begin(), cornerWallPositions.begin(), cornerWallPositions.end());

	return walls;
}

CoordList Algorithms::FindWallsInDirection(CoordList &floorPositions, CoordList directionList, Coord boundary)
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


vector<Wall> Algorithms::GetWallTiles(CoordList &floorPositions, CoordList &sideWallPositions, CoordList &cornerWallPositions)
{
	vector<Wall> walls;

	// Gets all the tiles for the side walls and adds them
	for (Coord position : sideWallPositions)
	{		
		string neighboursBinaryType = "";

		// searches each cardinal direction from the current position and if it
		// contains a floor sets the space to 1 otherwise sets it to 0
		for(var direction : Direction2D::CardinalDirections())
		{
			var neighbourPosition = position + direction;
			if (CoordInList(&floorPositions, neighbourPosition))
			{
				neighboursBinaryType += "1"; //contains a floor
			}
			else
			{
				neighboursBinaryType += "0"; // does not contain a floor
			}
		}

		var tile = GetWallType(neighboursBinaryType, false);

		if(tile != TileType::Empty)
			walls.push_back(Wall(position, tile));		
	}

	// Gets all the tiles for the corner walls and adds them
	for (Coord position : cornerWallPositions)
	{
		string neighboursBinaryType = "";

		// searches each cardinal direction from the current position and if it
		// contains a floor sets the space to 1 otherwise sets it to 0
		for (var direction : Direction2D::AllDirections())
		{
			var neighbourPosition = position + direction;
			if (CoordInList(&floorPositions, neighbourPosition))
			{
				neighboursBinaryType += "1"; //contains a floor
			}
			else
			{
				neighboursBinaryType += "0"; // does not contain a floor
			}
		}

		var tile = GetWallType(neighboursBinaryType, true);

		if (tile != TileType::Empty)
			walls.push_back(Wall(position, tile));
	}

	//system("pause");
	return walls;
}

TileType Algorithms::GetWallType(std::string binaryString, bool eightDirection)
{
	int type = stoi(binaryString, nullptr, 2); // converts binary string to integer
	TileType tile = TileType::Empty;

	// checks if the matching binary number is in any of the lists in the wall type helper and
	// if it is, sets the tile to that wall type

	// side walls
	if (!eightDirection)
	{
		if (WallTypes::TypeInList(WallTypes::wallTop(), type))
		{
			tile = TileType::Top;
		}
		else if (WallTypes::TypeInList(WallTypes::wallSideRight(), type))
		{
			tile = TileType::SideRight;
		}
		else if (WallTypes::TypeInList(WallTypes::wallSideLeft(), type))
		{
			tile = TileType::SideLeft;
		}
		else if (WallTypes::TypeInList(WallTypes::wallBottom(), type))
		{
			tile = TileType::Bottom;
		}
		else if (WallTypes::TypeInList(WallTypes::wallFull(), type))
		{
			tile = TileType::WallFull;
		}
		else
		{
			cout << "Wall not in list:" << binaryString << endl;
		}
	}
	else
	{
		// corner walls
		if (WallTypes::TypeInList(WallTypes::wallInnerCornerDownLeft(), type))
		{
			tile = TileType::InnerCornerDownLeft;
		}
		else if (WallTypes::TypeInList(WallTypes::wallInnerCornerDownRight(), type))
		{
			tile = TileType::InnerCornerDownRight;
		}
		else if (WallTypes::TypeInList(WallTypes::wallDiagonalCornerDownLeft(), type))
		{
			tile = TileType::DiagonalCornerDownLeft;
		}
		else if (WallTypes::TypeInList(WallTypes::wallDiagonalCornerDownRight(), type))
		{
			tile = TileType::DiagonalCornerDownRight;
		}
		else if (WallTypes::TypeInList(WallTypes::wallDiagonalCornerUpRight(), type))
		{
			tile = TileType::DiagonalCornerUpRight;
		}
		else if (WallTypes::TypeInList(WallTypes::wallDiagonalCornerUpLeft(), type))
		{
			tile = TileType::DiagonalCornerUpLeft;
		}
		else if (WallTypes::TypeInList(WallTypes::wallFullEightDirections(), type))
		{
			tile = TileType::WallFull;
		}
		else if (WallTypes::TypeInList(WallTypes::wallBottomEightDirections(), type))
		{
			tile = TileType::Bottom;
		}

	}

	return tile;
}