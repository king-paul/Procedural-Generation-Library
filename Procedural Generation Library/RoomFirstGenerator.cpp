#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

RoomFirstGenerator::RoomFirstGenerator(int dungeonWidth, int dungeonHeight, Coord startPosition, 
    int minRoomWidth, int minRoomHeight, int offset, bool randomWalkRooms, RandomWalkParameters paramaters) :
	RandomWalkGenerator(dungeonWidth, dungeonHeight, startPosition),
	m_minRoomWidth(minRoomWidth), m_minRoomHeight(minRoomHeight), m_offset(offset), m_randomWalkRooms(randomWalkRooms)
{
	
}

// Generates a series of bounding boxes using Binary space partitioning,
// creates rooms from the boxes then draws them
void RoomFirstGenerator::Generate()
{
    // creates a new list of bounding boxes by running the binary space partitioning algorithm
    var roomsList = Algorithms::BinarySpacePartitioning(Boundary(m_startPosition, { m_width, m_height }),
        m_minRoomWidth, m_minRoomHeight);

    // creates an empty list for coordinates for the floor then creates rooms
    CoordList floorPositions;
    if (m_randomWalkRooms) //create non square rooms if this is turned on
    {
        floorPositions = CreateRandomWalkRooms(roomsList);
    }
    else {
        floorPositions = CreateSimpleRooms(roomsList);
    }

    // creates a list of coordinates for the center of each room and then adds them all
    CoordList roomCenters;
    for(var room : roomsList)
    {
        roomCenters.Add(room.Center());
    }

    // creates corridors by connecting the rooms and then adds them to the floor tiles
    CoordList corridors = ConnectRooms(roomCenters);
    
    // adds the corridors to the floors
    floorPositions.insert(floorPositions.end(), corridors.begin(), corridors.end());

    // get wall tiles 
    var wallPositions = Algorithms::GetWalls(floorPositions, Coord(m_width, m_height));

    // add tiles to the map  
    AddToDungeon(&floorPositions, &wallPositions);
}

// Creates a set of rooms from the bounding boxes
CoordList RoomFirstGenerator::CreateSimpleRooms(vector<Boundary> rooms)
{
    CoordList floor;

    for(var room : rooms)
    {
        // iterates through all columns from the left offset
        // to the width of the room minus the right offset
        for (int col = m_offset; col < room.Size().x - m_offset; col++)
        {
            // iterates through all rows from the bottom offset
            // to the height of the room minus the top offset
            for (int row = m_offset; row < room.Size().y - m_offset; row++)
            {
                Coord position = room.min + Coord(col, row);
                floor.Add(position);
            }
        }
    }

    return floor;
}

// Generates a series of rooms in the binary space partition that are not rectangles
// by using the random walk algorithm
CoordList RoomFirstGenerator::CreateRandomWalkRooms(vector<Boundary> rooms)
{
    CoordList floor;
    for (int i = 0; i < rooms.size(); i++)
    {
        var roomBounds = rooms[i]; // the boundary area of the next room
        var roomCenter = Coord(roomBounds.Center().x, roomBounds.Center().y);
        var roomFloor = RunRandomWalk(roomCenter);

        for(var position : roomFloor)
        {
            // checks that the position is inside the boundary and if it is adds it to the floors
            if (position.x >= (roomBounds.min.x + m_offset) && position.x <= (roomBounds.min.x - m_offset) &&
                position.y >= (roomBounds.min.y - m_offset) && position.y <= (roomBounds.min.y - m_offset))
            {
                floor.Add(position);
            }

        }
    }

    return floor;
}

// Connects all the rooms together by creating corridors from the center or the rooms
CoordList RoomFirstGenerator::ConnectRooms(CoordList roomCenters)
{
	CoordList corridors;
    CoordList::iterator it;

    // randomly select a room centre from the list then remove it form the list
    int randomIndex = rand() % roomCenters.size();
    
    var currentRoomCenter = roomCenters[randomIndex];
    roomCenters.erase(roomCenters.begin() + randomIndex);

    // while there are still room centers ro be romove
    while (roomCenters.size() > 0)
    {
        // finds the closest room and remove it
        Coord closest = FindClosestPointTo(currentRoomCenter, roomCenters);

        CoordList::iterator index = std::find(roomCenters.begin(), roomCenters.end(), closest);
        roomCenters.erase(index);

        // creats a new corridor from the closest point and makes it the new room centre
        CoordList newCorridor = CreateCorridor(currentRoomCenter, closest);
        currentRoomCenter = closest;

        // adds the new corridor to the existing ones
        corridors.insert(corridors.end(), newCorridor.begin(), newCorridor.end());
    }

    return corridors;
}

// Connects a corridor from the current room to the nearest one
CoordList RoomFirstGenerator::CreateCorridor(Coord currentRoomCenter, Coord destination)
{
    CoordList corridor;
    var position = currentRoomCenter;
    corridor.Add(position); // add the center tile to the corridor tiles

    // keeps adding new corridor positions until
    // it matches up with the destinations y position
    while (position.y != destination.y)
    {
        // if the destination is above the position move up
        if (destination.y > position.y)
        {
            position += Direction2D::Up();
              
        }
        // if the destination is below the position move down
        else if (destination.y < position.y)
        {
            position += Direction2D::Down();
        }

        corridor.Add(position);
    }

    // keeps adding new corridor positions until
    // it matches up with the destinations x position
    while (position.x != destination.x)
    {
        if (destination.x > position.x)
        {
            position += Direction2D::Right();
        }
        else if (destination.x < position.x)
        {
            position += Direction2D::Left();
        }

        corridor.Add(position);
    }

    return corridor;
}

// Finds the closet room to the current room and returns the coordinates of the centre of that room
Coord RoomFirstGenerator::FindClosestPointTo(Coord currentRoomCenter, CoordList roomCenters)
{
    Coord closest = {0, 0};
    float distance = FLT_MAX;

    // calculates the distance from each room centre to the current room center
    for(var position : roomCenters)
    {
        float currentDistance = Coord::DistanceBetween(position, currentRoomCenter);

        // if the distance is less than the smallest distance then
        // set the smallest distance to the current one
        if (currentDistance < distance)
        {
            distance = currentDistance;
            closest = position;
        }
    }

    return closest;
}