#include "CaveGenerator.h"

CaveGenerator::CaveGenerator(int width, int height, int fillPercent, int smoothingIterations, 
    int borderSize, int wallThresholdSize, int roomThresholdSize, bool useRandomSeed, string seed)
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