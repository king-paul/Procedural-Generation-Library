#include "CaveGenerator.h"

CaveGenerator::CaveGenerator(int width, int height, int fillPercent, int smoothingIterations, 
    int borderSize, int wallThresholdSize, int roomThresholdSize, int passageWidth, bool useRandomSeed, string seed)
{
    m_width = width;
    m_height = height;
    m_randomFillPercent = fillPercent;
    m_smoothingIterations = smoothingIterations;
    m_useRandomSeed = useRandomSeed;

    if (m_useRandomSeed)
        m_seed = rand();
    else
        m_seed = seed;

    m_borderSize = 1;
    m_wallThresholdSize = 50;
    m_roomThresholdSize = 50;
}

Array2D<int>* CaveGenerator::GenerateMap()
{
    m_map = new Array2D<int>(m_width, m_height);
    RandomFillMap();

    // smooth iterations
    for (int i = 0; i < m_smoothingIterations; i++)
    {
        SmoothMap();
    }

    return m_map;
}

void CaveGenerator::ProcessMap()
{
    vector<vector<Coord>> wallRegions = GetRegions(1);

    // remove walls within treshold from the map
    for(vector<Coord> wallRegion : wallRegions)
    {
        if (wallRegion.size() < m_wallThresholdSize)
        {
            for(Coord tile : wallRegion)
            {
                m_map->get(tile.x, tile.y) = 0;
            }
        }
    }

    vector<vector<Coord>> roomRegions = GetRegions(0);

    // remove rooms within regions from the map
    for(vector<Coord> roomRegion : roomRegions)
    {
        // if the region is less than the treshold change the space to a wall
        if (roomRegion.size() < m_roomThresholdSize)
        {
            for(Coord tile : roomRegion)
            {
                m_map->get(tile.x, tile.y) = 1;
            }
        }
    }
}

vector<vector<Coord>> CaveGenerator::GetRegions(int tileType)
{
    vector<vector<Coord>> regions;
    Array2D<int> mapFlags(m_width, m_height);

    // iterates through the map finding all tiles that match the type
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            if (mapFlags.get(x, y) == 0 && mapFlags.get(x, y) == tileType)
            {
                // gets all tiles within the region and add them to the vector
                vector<Coord> newRegion = GetRegionTiles(x, y);
                regions.Add(newRegion);

                // mark all tiles in the flags as being looked at
                for(Coord tile : newRegion)
                {
                    mapFlags.get(tile.x, tile.y) = 1;
                }
            }
        }
    }

    return regions;
}

vector<Coord> CaveGenerator::GetRegionTiles(int startX, int startY)
{
    vector<Coord> tiles;
    // determines if a tile has been chacked or not
    // 1 is it has, 0 if it hasn't
    Array2D<int> mapFlags(m_width, m_height);

    int tileType = m_map->get(startX, startY); // the type of tile being stored in the map

    // create a queue of coordinates and add the starting one to it
    queue<Coord> queue;
    queue.push(Coord(startX, startY));
    mapFlags.get(startX, startY) = 1;

    // while there are still tiles in the queue
    // get the first tile in the queue and remove it
    while (queue.size() > 0)
    {
        Coord tile = queue.back();
        queue.pop();

        tiles.Add(tile);

        // looks at all the adjacent tiles
        for (int x = tile.x - 1; x <= tile.x + 1; x++)
        {
            for (int y = tile.y - 1; y <= tile.y + 1; y++)
            {
                // ensure that the tile is in the range of the map and that it is no a diagonal
                if (IsInMapRange(x, y) && (y == tile.y || x == tile.x))
                {
                    if (mapFlags.get(x, y) == 0 && m_map->get(x, y) == tileType)
                    {
                        mapFlags.get(x, y) = 1; // marks the tile as checked
                        queue.push(Coord(x, y));
                    }
                }
            }
        }
    }

    return tiles;
}

bool CaveGenerator::IsInMapRange(int x, int y)
{
    return x >= 0 && x < m_width&& y >= 0 && y < m_height;
}

void CaveGenerator::RandomFillMap()
{
    if (m_useRandomSeed)
    {
        srand(time_t(NULL));
        //m_seed = rand();
        //seed = Timer.time.ToString();
    }    

    // iterate through the graph creating cubes
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            if (x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1)
            {
                m_map->get(x, y) = 1;
            }
            else
            {
                m_map->get(x, y) = ((rand() % 100) < m_randomFillPercent) ? 1 : 0;
            }

            m_map->get(x, y) = ((rand() % 100) < m_randomFillPercent) ? 1 : 0;
        }
    }

}

void CaveGenerator::SmoothMap()
{
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            int neighboutWallTiles = GetSurroundingWallCount(x, y);

            if (neighboutWallTiles > 4)
                m_map->get(x, y) = 1;
            else if (neighboutWallTiles < 4)
                m_map->get(x, y) = 0;
        }
    }

}

int CaveGenerator::GetSurroundingWallCount(int gridX, int gridY)
{
    int wallCount = 0;

    // loops trough 3x3 grid
    for (int neighbourX = gridX - 1; neighbourX <= gridX + 1; neighbourX++)
    {
        for (int neighbourY = gridY - 1; neighbourY <= gridY + 1; neighbourY++)
        {
            // make sure we are inside the map
            if (IsInMapRange(neighbourX, neighbourY))
            {
                // make sure that it is not the current tile
                if (neighbourX != gridX || neighbourY != gridY)
                {
                    wallCount += m_map->get(neighbourX, neighbourY); // add to walls
                }
            }
            else
            {
                wallCount++;
            }

        }
    }

    return wallCount;
}

void CaveGenerator::ConnectClosestRooms(vector<Room*> *allRooms, bool forceAccess)
{
    vector<Room*> roomsConnected; // rooms connected to main room
    vector<Room*> roomsDisconnected; // rooms not accessible from main room
    int shortestDistance = 0;

    // coordinates of tiles at the each end of the best distance
    Coord closestPosA;
    Coord closestPosB;
    // the two rooms in the best distance
    Room* closestRoomA;
    Room* closestRoomB;
    bool connectionFound = false;

    // if we are forcing accessibility from the main room then the two
    // then connect the rooms
    if (forceAccess)
    {
        for(Room* room : *allRooms)
        {
            // if the room can be accessed from the main room
            // add it to the connected list else add it to the disconnect list
            if (room->IsAccessibleFromMain())
                roomsConnected.Add(room);
            else
                roomsDisconnected.Add(room);
        }
    }
    else // otherwise both lists contain all rooms
    {
        roomsConnected = *allRooms;
        roomsDisconnected = *allRooms;
    }

    // iterates through all rooms in the list and
    // compares them with all other rooms
    for (Room* roomA : roomsConnected)
    {
        // if we are not forcing accessibity then the
        // next room is not connected yet
        if (!forceAccess) {
            connectionFound = false;

            // if another room is connected to this room then go to next one
            if (roomA->HasConnections()) 
                continue;            
        }

        for (Room* roomB : roomsDisconnected)
        {
            // if both rooms are the same or A is already connected to B then
            // skip to next room on list   
            if (roomA == roomB || roomA->IsConnectedTo(roomB))
                continue;

            // iterates through all indexes up until the total number of edge tiles has
            // been reached
            for (int roomAindex = 0; roomAindex < roomA->GetEdgeCount(); roomAindex++)
            {
                Coord pointA = roomA->GetEdgeTile(roomAindex);

                for (int roomBindex = 0; roomBindex < roomB->GetEdgeCount(); roomBindex++)
                {                    
                    Coord pointB = roomB->GetEdgeTile(roomBindex);

                    // finds the distance between the rooms using pythagoras theorem and
                    // converts it to the nearest integer
                    int distanceBetweenRooms = (int)( pow(pointA.x - pointB.x, 2) + pow(pointA.y - pointB.y, 2) );

                    // if a new shortest distance has been found or no connection has been found yet
                    // update all the short distance variables
                    if (distanceBetweenRooms < shortestDistance || !connectionFound)
                    {
                        shortestDistance = distanceBetweenRooms;
                        connectionFound = true;
                        closestPosA = pointA;
                        closestPosB = pointB;
                        closestRoomA = roomA;
                        closestRoomB = roomB;
                    }

                }
            }

        } // end of loop B

        // if connection was found create a passage between the 2 shortest points
        // and we are not forcing access from the main room
        // create a passage
        if (connectionFound && !forceAccess)
        {
            CreatePassage(closestRoomA, closestRoomB, closestPosA, closestPosB);
        }

    } // end of loop A

    // if access from main rooms is not enforce then 
    // all rooms will be forced to find a connection to a room
    if (!forceAccess)
    {
        ConnectClosestRooms(allRooms, true);
    }

}

void CaveGenerator::CreatePassage(Room* roomA, Room* roomB, Coord tileA, Coord tileB)
{
    Room::ConnectRooms(roomA, roomB);

    // gets the line connecting the two coordinates
    vector<Coord> path = CreateLine(tileA, tileB);

    // carve a path in each position in the line
    for(Coord coord : path)
    {
        CreateCircle(coord, m_passageWidth);
    }
}

void CaveGenerator::CreateCircle(Coord coordinate, int radius)
{
    for (int x = -radius; x <= radius; x++)
    {
        for (int y = -radius; y <= radius; y++)
        {
            // check if we are inside the circle
            if (x * x + y * y <= radius * radius)
            {
                // set the new grid position to x+1 y+1
                int gridX = coordinate.x + x;
                int gridY = coordinate.y + y;

                // if the position is inside the boundary of the cave
                // set the position to a vacent space
                if (IsInMapRange(gridX, gridY))
                    m_map->set(gridX, gridY, 0);
            }
        }

    }
}

std::vector<Coord> CaveGenerator::CreateLine(Coord fromPoint, Coord toPoint)
{
    vector<Coord> line;

    int x = fromPoint.x;
    int y = fromPoint.y;

    bool inverted = false;
    int dx = toPoint.x - fromPoint.x;
    int dy = toPoint.y - fromPoint.y;

    // return value is 1 when positive or zero, -1 when negative
    int step = sign(dx); // determines if dx is positive or negative
    int gradientStep = sign(dy); // determines if dy is positive or negative

    int longest = abs(dx); // positive value of dx
    int shortest = abs(dy); // positive value of dy

    // if dx is shorter than dy then invert the values
    if (longest < shortest)
    {
        inverted = true;
        longest = abs(dy);
        shortest = abs(dx);

        step = sign(dy);
        gradientStep = sign(dx);
    }

    int gradientAccumulation = longest / 2;

    // adds coordinates to the line until the length has been reached
    for (int i = 0; i < longest; i++) {
        line.Add(Coord(x, y));

        // increment x or y based on whether it is inverted
        if (inverted)
        {
            y += step;
        }
        else
        {
            x += step;
        }

        gradientAccumulation += shortest;

        if (gradientAccumulation >= longest)
        {
            if (inverted)
                x += gradientStep;
            else
                y += gradientStep;

            gradientAccumulation -= longest;
        }

    }

    return line;
}

Vector3 CaveGenerator::CoordToWorldPoint(Coord tile)
{
    return Vector3(-m_width / 2 + .5f + tile.x, 2, -m_height / 2 + .5f + tile.y);
}
