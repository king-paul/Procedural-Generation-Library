#include "CaveGenerator.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace ProceduralGeneration;

CaveGenerator::CaveGenerator(int width, int height, int fillPercent, int smoothingIterations, 
    int borderSize, int wallThresholdSize, int roomThresholdSize, int passageWidth, bool forceAccessToMain,
    bool useRandomSeed, int seed)
{
    m_width = width;
    m_height = height;
    m_randomFillPercent = fillPercent;
    m_smoothingIterations = smoothingIterations;
    m_useRandomSeed = useRandomSeed;

    if (m_useRandomSeed)
        m_seed = chrono::steady_clock::now().time_since_epoch().count();
    else
        m_seed = seed;

    m_borderSize = 1;
    m_wallThresholdSize = 50;
    m_roomThresholdSize = 50;
    m_passageWidth = passageWidth;
    m_forceAccessToMain = forceAccessToMain;

    m_map = new Array2D<int>(m_width, m_height);

    m_randomGenerator = new PseudoRandom(0, 100, m_seed);
}

void CaveGenerator::GenerateMap()
{   
    m_map->clear();
    for (Room* room : m_rooms)
        delete room;
    m_rooms.clear();

    RandomFillMap();  

    // smooth iterations
    for (int i = 0; i < m_smoothingIterations; i++)
    {
        SmoothMap();
    }

    ProcessMap();

    //Array2D<int> borderedMap(m_width + m_borderSize * 2, m_height + m_borderSize * 2);
    //m_squareGrid = new SquareGrid(&borderedMap, 1);
}

void CaveGenerator::ProcessMap()
{
    vector<vector<Coord>> isolatedWalls = GetAllRegions(1);

    // remove walls within treshold from the map
    for(vector<Coord> isolatedWall : isolatedWalls)
    {
        if (isolatedWall.size() < m_wallThresholdSize)
        {
            for (Coord tile : isolatedWall)
                DrawAtPos(tile.x, tile.y, '*', RED);

            for(Coord tile : isolatedWall)
            {
                m_map->at(tile.x, tile.y) = 0;
                DrawAtPos(tile.x, tile.y, '.', CYAN);
            }
        }
    }

    // Find rooms in the cave and add them
    vector<vector<Coord>> roomSections = GetAllRegions(0);   

    // remove rooms within regions from the map
    for(vector<Coord> room : roomSections)
    {
        //PrintRoomOnMap(room);

        // if the region is less than the treshold change the space to a wall
        if (room.size() < m_roomThresholdSize)
        {
            for (Coord tile : room)
                DrawAtPos(tile.x, tile.y, '.', RED);

            for(Coord tile : room)
            {
                m_map->set(tile.x, tile.y, 1);
                DrawAtPos(tile.x, tile.y, '*', BROWN);
            }            

        } // otherwise add a new room
        else
        {
            m_rooms.Add(new Room(room, m_map));
        }

    }
    
    // sort the rooms from largest to smallest so that the largest room is at the start
    std::sort(m_rooms.begin(), m_rooms.end(), [](const Room* a, const Room* b) {
            return *a > *b;
        }
    );

    if (m_rooms.size() > 0)
    {
        // sets the larget room to be the main room
        m_rooms[0]->SetAsMainRoom();

        ConnectClosestRooms(&m_rooms);
    }
    
}

Array2D<int> CaveGenerator::CreateBorderedMap()
{
    int newWidth = m_width + m_borderSize * 2;
    int newHeight = m_height + m_borderSize * 2;

    Array2D<int> newMap(newWidth, newHeight);

    // iterate through the bordered map array
    for (int x = 0; x < newWidth; x++)
    {
        for (int y = 0; y < newHeight; y++)
        {
            // if we are inside the borders then retrieve the values from the map at that position
            if (x >= m_borderSize && x < m_width + m_borderSize && y >= m_borderSize && y < m_height + m_borderSize)
            {
                newMap.at(x, y) = m_map->at(x - m_borderSize, y - m_borderSize);
            }
            else // if we are outside the map set the value to a wall
            {
                newMap.at(x, y) = 1;
            }
        }
    }

    return newMap;
}

vector<vector<Coord>> CaveGenerator::GetAllRegions(int type)
{
    vector<vector<Coord>> isolatedRegions;
    Array2D<bool> checkedTiles(m_width, m_height, false);

    // iterates through the map finding all tiles that match the type
    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            if (checkedTiles.at(x, y) == false && m_map->at(x, y) == type)
            {
                // gets all tiles within one region and add them to the vector
                vector<Coord> newRegion = GetRegionCoords(x, y);
                isolatedRegions.Add(newRegion);               

                // mark all tiles in the the new region as being looked at
                for(Coord tile : newRegion)
                {
                    checkedTiles.set(tile.x, tile.y, 1);
                    
                    /*
                    if (type == 0)
                        DrawAtPos(tile.x, tile.y, '.', RED);
                    else if (type == 1)
                        DrawAtPos(tile.x, tile.y, '*', RED);*/
                }
            }
        }
    }

    return isolatedRegions;
}

vector<Coord> CaveGenerator::GetRegionCoords(int startX, int startY)
{
    vector<Coord> tiles;
    // determines if a tile has been chacked or not
    // 1 is it has, 0 if it hasn't
    Array2D<int> checkedTiles(m_width, m_height, NULL);
    //DrawCheckedPositions(&mapFlags);

    int tileType = m_map->at(startX, startY); // the type of tile being stored in the map

    // create a queue of coordinates and add the starting one to it
    queue<Coord> queue;
    queue.push(Coord(startX, startY));
    checkedTiles.set(startX, startY, 1);
    //DrawCheckedPositions(&mapFlags);

    // while there are still tiles in the queue
    // get the first tile in the queue and remove it
    while (queue.size() > 0)
    {
        //PrintCave(queue);
        Coord tile = queue.front();
        queue.pop();

        tiles.Add(tile);
        //PrintCave(tiles, tile, x, y);
        //PrintRoomOnMap(tiles);     

        // looks at all the adjacent tiles by by searching a square grid
        for (int x = tile.x - 1; x <= tile.x + 1; x++)
        {
            for (int y = tile.y - 1; y <= tile.y + 1; y++)
            {                
                //PrintCaveWithGrid(tiles, tile, x, y);

                // ensure that the tile is in the range of the map and that it is no a diagonal
                if (IsInMapRange(x, y) && (y == tile.y || x == tile.x))
                {
                    //DrawAtPos(x, y, 'o');
                    // check that the space hase not already been processed and
                    // the tile at the grid position matches the type
                    if (checkedTiles.at(x, y) == 0 && m_map->at(x, y) == tileType)
                    {                       
                        checkedTiles.set(x, y, 1); // marks the tile as checked
                        queue.push(Coord(x, y));
                        //DrawAtPos(x, y, '+');
                    }

                    //DrawAtPos(x, y, '*');
                }

                //DrawCheckedPositions(&mapFlags);
                
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
    // iterate through the graph creating noise
    for (int y = 0; y < m_height; y++)      
    {
        for (int x = 0; x < m_width; x++)
        {
            if (x == 0 || x == m_width - 1 || y == 0 || y == m_height - 1)
            {
                m_map->set(x, y, 1);
                DrawAtPos(x, y, '*', BROWN);
            }
            else
            {
                float randomValue = m_randomGenerator->GetValue(); //rand() % 100;

                if (randomValue < m_randomFillPercent) {
                    m_map->set(x, y, 1);// ? 1 : 0;
                    DrawAtPos(x, y, '*', BROWN);
                }
                else {
                    m_map->set(x, y, 0);
                    DrawAtPos(x, y, '.', CYAN);
                }

            }

        }
    }

}

void CaveGenerator::SmoothMap()
{
    for (int y = 0; y < m_height; y++)    
    {
        for (int x = 0; x < m_width; x++)
        {
            int neighboutWallTiles = GetSurroundingWallCount(x, y);

            if (neighboutWallTiles > 4)
            {
                m_map->at(x, y) = 1;                
                DrawAtPos(x, y, '*', BROWN);
            }
            else if (neighboutWallTiles < 4)
            {
                m_map->at(x, y) = 0;                
                DrawAtPos(x, y, '.', CYAN);
            }
        }
        
    }

}

int CaveGenerator::GetSurroundingWallCount(int gridX, int gridY)
{
    int wallCount = 0;

    // loops trough 3x3 grid
    for (int neighbourY = gridY - 1; neighbourY <= gridY + 1; neighbourY++)    
    {
        for (int neighbourX = gridX - 1; neighbourX <= gridX + 1; neighbourX++)
        {
            // make sure we are inside the map
            if (IsInMapRange(neighbourX, neighbourY))
            {
                // make sure that it is not the current tile
                if (neighbourX != gridX || neighbourY != gridY)
                {
                    wallCount += m_map->at(neighbourX, neighbourY); // add to walls
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
    Room* closestRoomA = nullptr;
    Room* closestRoomB = nullptr;
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
        HighlightRoom(roomA, 31);

        // if we are not forcing accessibity then the
        // next room is not connected yet
        if (!forceAccess) {
            connectionFound = false;

            // if another room is connected to this room then go to next one
            if (roomA->HasConnections())
            {
                HighlightRoom(roomA, 36);
                continue;
            }
        }

        for (Room* roomB : roomsDisconnected)
        {       
            // if both rooms are the same or A is already connected to B then
            // skip to next room on list   
            if (roomA == roomB || roomA->IsConnectedTo(roomB))
                continue;

            if (connectionFound)
            {
                DrawAtPos(closestPosA.x, closestPosA.y, '.', 36);
                DrawAtPos(closestPosB.x, closestPosB.y, '.', 36);
            }

            HighlightRoom(roomB, 32);

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
                        //if (connectionFound)
                        //{
                            //DrawAtPos(closestPosA.x, closestPosA.y, '.', 32);
                            //DrawAtPos(closestPosB.x, closestPosB.y, '.', 32);
                        //}

                        shortestDistance = distanceBetweenRooms;
                        connectionFound = true;
                        closestPosA = pointA;
                        closestPosB = pointB;
                        closestRoomA = roomA;
                        closestRoomB = roomB;

                        //DrawAtPos(closestPosA.x, closestPosA.y, 'A');
                        //DrawAtPos(closestPosB.x, closestPosB.y, 'B');
                    }

                }
            }

            DrawAtPos(closestPosA.x, closestPosA.y, 'A');
            DrawAtPos(closestPosB.x, closestPosB.y, 'B');
            HighlightRoom(roomB, 36);

        } // end of loop B

        // if connection was found create a passage between the 2 shortest points
        // and we are not forcing access from the main room
        // create a passage
        if (connectionFound && forceAccess)
        {
            // create a passage between the 2 shortest points
            CreatePassage(closestRoomA, closestRoomB, closestPosA, closestPosB);

            // runs another iteration of this function recursively
            ConnectClosestRooms(allRooms, true);
        }

        HighlightRoom(roomA, 36);

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
    HighlightRoom(roomA, 31);
    HighlightRoom(roomB, 32);

    Room::ConnectRooms(roomA, roomB);

    // gets the line connecting the two coordinates
    vector<Coord> path = CreateLine(tileA, tileB);

    DrawAtPos(tileA.x, tileA.y, 'A');
    DrawAtPos(tileB.x, tileB.y, 'B');

    // carve a path in each position in the line
    for(Coord coord : path)
    {
        CreateCircle(coord, m_passageWidth);
    }

    DrawAtPos(tileA.x, tileA.y, '.', 36);
    DrawAtPos(tileB.x, tileB.y, '.', 36);

    HighlightRoom(roomA, 36);
    HighlightRoom(roomB, 36);
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
                {
                    m_map->set(gridX, gridY, 0);
                    DrawAtPos(gridX, gridY, '.', 36);
                }
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