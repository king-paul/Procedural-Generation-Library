#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

RoomFirstGenerator::RoomFirstGenerator(int minRoomWidth = 10, int minRoomHeight = 10,
	int dungeonWidth = 20, int dungeonHeight = 20, Coord startPosition = { 0, 0 }, bool randomWalkRooms = false) :
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