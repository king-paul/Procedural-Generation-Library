#include "DungeonGenerator.h"

using namespace ProceduralGeneration;


CorridorFirstGenerator::CorridorFirstGenerator(int dungeonWidth, int dungeonHeight, Coord startPosition, 
	int corridorLength, int totalCorridors, float roomPercent, RandomWalkParameters parameters)
	: RandomWalkGenerator(dungeonWidth, dungeonHeight, startPosition, 
        parameters.iterations, parameters.walkLength, parameters.startRandomly),
	m_corridorLength(corridorLength), m_TotalCorridors(totalCorridors), m_roomPercent(roomPercent)
{

}

// runs the generation algorithm for this class
void CorridorFirstGenerator::Generate()
{
    CoordList floorPositions;
    CoordList potentialRoomPositions;

    // generates the corridors
    CreateCorridors(floorPositions, potentialRoomPositions);

    //AddToDungeon(&floorPositions, 1);
    //DrawDungeon();

    // generates a series of rooms from the potential positions
    CoordList roomPositions = CreateRooms(&potentialRoomPositions);

    //AddToDungeon(&roomPositions, 1);
    //DrawDungeon();

    // locates all dead ends on the map
    CoordList deadEnds = FindAllDeadEnds(&floorPositions);
    // places a room at each dead end
    CreateRoomsAtDeadEnd(&deadEnds, roomPositions);

    //AddToDungeon(&roomPositions, 1);
    //DrawDungeon();

    // add rooms to floor
    floorPositions.insert(floorPositions.end(), roomPositions.begin(), roomPositions.end());
        
    // get wall tiles 
    CoordList wallPositions = Algorithms::GetWalls(floorPositions, Coord(m_width, m_height));
    
    // add tiles to the map
    //AddToDungeon(&wallPositions, -1);    
    AddToDungeon(&floorPositions, &wallPositions);
}

// generates a series of corridors equal to the corridor count
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

        DrawMap();
        */
    }
    
}

// randomly selects spaces from the potential room positions to create rooms in
// and then returns those spaces
CoordList CorridorFirstGenerator::CreateRooms(CoordList* potentialRoomPositions)
{
    CoordList roomPositions = *potentialRoomPositions;
    CoordList roomFloors;

    // generates a random number of rooms to create
    int totalRooms = (int) ((float)potentialRoomPositions->size() * m_roomPercent);

    // make the list of potential positions random
    random_shuffle(roomPositions.begin(), roomPositions.end());

    // remove positions until the list is the same size as the total rooms to create
    while (roomPositions.size() > totalRooms)
    {
        roomPositions.pop_back();
    }    

    // generates a room at each random room position by running the random walk algorithm
    for (Coord roomPosition : roomPositions)
    {
        m_position = roomPosition;
        var roomFloor = RunRandomWalk(roomPosition);

        //AddToDungeon(&roomFloor, 1);
        //DrawDungeon();

        roomFloors.insert(roomFloors.end(), roomFloor.begin(), roomFloor.end());
    }

    return roomFloors;
}

// Generates a room and each dead end on the map
void CorridorFirstGenerator::CreateRoomsAtDeadEnd(CoordList* deadEnds, CoordList& roomFloors)
{
    for(var position : *deadEnds)
    {
        // if there is no floor touching the dead end poisition
        if (!CoordInList(&roomFloors, position))
        {
            var room = RunRandomWalk(position);
            roomFloors.insert(roomFloors.end(), room.begin(), room.end());            
        }
    }
}

// Finds all dead ends on the map using the floor positions and returns the dead end positions
CoordList CorridorFirstGenerator::FindAllDeadEnds(CoordList* floorPositions)
{
    CoordList deadEnds;

    for(var position : *floorPositions)
    {
        int neighboursCount = 0;
        for(var direction : Direction2D::CardinalDirections())
        {
            // if there is a floor at the position increase the neighbours
            if (CoordInList(floorPositions, position + direction))
                neighboursCount++;
        }

        if (neighboursCount == 1)
            deadEnds.Add(position);
    }

    return deadEnds;
}