#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

RandomWalkDungeonGenerator::RandomWalkDungeonGenerator(int size, Coord position, int iterations, int walkLength, bool startRandomly) 
   : DungeonGenerator(size)
{
    /*
    m_size = size;
    m_map = new int* [m_size];

    for (int i = 0; i < size; i++)
        m_map[i] = new int(0);*/

	m_position = position;
	m_iterations = iterations;
	m_walkLength = walkLength;
	m_startRandomly = startRandomly;    
}

void RandomWalkDungeonGenerator::Generate()
{
	InitMap();
    //DrawMap();
	CoordList floorPositions = RunRandomWalk();
    CoordList wallPositions = Algorithms::GetWalls(floorPositions);

    for (int y = 0; y < m_size; y++)
    {
        m_map.push_back(vector<int>());

        for (int x = 0; x < m_size; x++)
        {            
            for (Coord position : floorPositions)
            {
                if (position.x == x && position.y == y)
                {
                    //m_floorCoords[0].Add(x);
                    //m_floorCoords[1].Add(y);
                    m_map[y][x] = 1;
                    //m_map[y].push_back(1);
                    //DrawMap();
                    continue;
                }
            }            
            
            for (Coord position : wallPositions)
            {
                if (position.x == x && position.y == y)
                {
                    //m_wallCoords[0].Add(x);
                    //m_wallCoords[1].Add(y);
                    m_map[y][x] = -1;
                    continue;
                }
            }

            //m_map[y][x] = 0;
            
            //m_map[y].push_back(0);            
        }

    }

}

CoordList RandomWalkDungeonGenerator::RunRandomWalk()
{
    var currentPosition = m_position;
    CoordList floorPositions;

    for (int i = 0; i < m_iterations; i++)
    {
        // calls function recursively
        CoordList newPositions = Algorithms::RandomWalk(currentPosition, m_walkLength);

        floorPositions.insert(floorPositions.end(), newPositions.begin(), newPositions.end());
        //floorPositions.push_back(); // joins two hashsets together

        // starts new path at a random point on the existing one if this option is turned on
        if (m_startRandomly)
            currentPosition = floorPositions[rand() % floorPositions.size()];
    }

    return floorPositions;
}