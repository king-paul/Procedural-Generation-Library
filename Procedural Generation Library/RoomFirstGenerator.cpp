#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

RoomFirstGenerator::RoomFirstGenerator(int minRoomWidth, int minRoomHeight, int dungeonWidth, int dungeonHeight,
	Coord startPosition, bool randomWalkRooms, RandomWalkParameters paramaters) :
	RandomWalkGenerator(dungeonWidth, dungeonHeight, startPosition)
{
	
}


void RoomFirstGenerator::CreateRooms()
{

}

CoordList RoomFirstGenerator::ConnectRooms()
{
	return CoordList();
}

CoordList RoomFirstGenerator::CreateCorridor(CoordList roomCenters)
{
	return CoordList();
}

CoordList RoomFirstGenerator::FindClosestPointTo(CoordList currentRoomCenter, CoordList roomCenters)
{
	return CoordList();
}

CoordList RoomFirstGenerator::CreateSimpleRooms(CoordList roomsList)
{
	return CoordList();
}

CoordList RoomFirstGenerator::CreateRoomsRandomly(CoordList roomsList)
{
	return CoordList();
}