#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

CorridorFirstGenerator::CorridorFirstGenerator(int dungeonWidth, int dungeonHeight, Coord startPosition, 
	int corridorLength, int totalCorridors, float roomPercent)
	: DungeonGenerator(dungeonWidth, dungeonHeight, startPosition),
	m_corridorLength(corridorLength), m_TotalCorridors(totalCorridors), m_roomPercent(roomPercent)
{

}

void CorridorFirstGenerator::Generate()
{
    CoordList floorPositions;
    CoordList potentialRoomPositions;

    // generates the corridors
    CreateCorridors(floorPositions, potentialRoomPositions);

    // generates a series of rooms from the potential positions
    //CoordList roomPositions = CreateRooms(&potentialRoomPositions);

    // locates all dead ends on the map
    //CoordList deadEnds = FindAllDeadEnds(&floorPositions);
    // places a room at each dead end
    //CreateRoomsAtDeadEnd(&deadEnds, &roomPositions);

    // add rooms to floor
    //floorPositions.insert(floorPositions.end(), roomPositions.begin(), roomPositions.end());
        
    // get wall tiles 
    CoordList wallPositions = Algorithms::GetWalls(floorPositions);
    
    // add tiles to the map
    //BuildMap(&floorPositions, 1);
    BuildMap(&floorPositions, &wallPositions);
}

void CorridorFirstGenerator::CreateCorridors(CoordList& floorPositions, CoordList& potentialRoomPositions)
{
    var currentPosition = m_startPosition;
    potentialRoomPositions.Add(currentPosition);

    for (int i = 0; i < m_TotalCorridors; i++)
    {
        // generates a corridor using the procedural algorithms and adds it to the floor positions
        // and the end of the corridor to the potential room positions
        var corridor = Algorithms::RandomWalkCorridor(m_width, m_height,
                                                             currentPosition, m_corridorLength);

        currentPosition = corridor[corridor.size() - 1]; // moves to the end of the corridor

        potentialRoomPositions.Add(currentPosition);
        floorPositions.insert(floorPositions.end(), corridor.begin(), corridor.end());

        // for debugging
        /*
        for(Coord position : floorPositions)
            m_map[position.y][position.x] = 1;

        DrawMap();*/
    }
}

CoordList CorridorFirstGenerator::CreateRooms(CoordList* potentialRoomPositions)
{
    // generates a random number of rooms to create
    CoordList roomPositions;

    int roomsToCreateCount = (int) ((float)potentialRoomPositions->size() * m_roomPercent);

    //std::sort()

    // creates a unituqe identifier and returns a list of coordinates for the rooms to create
    // The rooms are ordered by the unique identifier value using the LinQ library
    //List<Vector2Int> roomsToCreate = potentialRoomPositions.OrderBy(x = > Guid.NewGuid()).Take(roomsToCreateCount).ToList();

    // generates a room at each random room position by running the random walk algorithm
    /*for (Coord roomPosition : roomsToCreate)
    {

        var roomFloor = RunRandomWalk(randomWalkParameters, roomPosition);

        roomPositions.insert(roomPositions.begin(), roomFloor.begin(). roomFloor.end());
    }*/

    return roomPositions;
}

void CorridorFirstGenerator::CreateRoomsAtDeadEnd(CoordList* deadEnds, CoordList* roomFloors)
{

}

CoordList CorridorFirstGenerator::FindAllDeadEnds(CoordList* floorPositions)
{
    return CoordList();
}