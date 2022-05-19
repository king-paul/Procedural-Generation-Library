#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

RandomWalkGenerator::RandomWalkGenerator(int width, int height, Coord position, 
int iterations, int walkLength, bool startRandomly)  : DungeonGenerator(width, height, position)
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

void RandomWalkGenerator::Generate()
{
	InitMap();
    //DrawMap();
	CoordList floorPositions = RunRandomWalk();
    CoordList wallPositions = Algorithms::GetWalls(floorPositions);

    for (int y = 0; y < m_height; y++)
    {
        m_map.push_back(vector<int>());

        for (int x = 0; x < m_width; x++)
        {            
            for (Coord position : floorPositions)
            {
                if (position.x == x && position.y == y)
                {
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
                    m_map[y][x] = -1;
                    continue;
                }
            }

            //m_map[y][x] = 0;            
            //m_map[y].push_back(0);            
        }
    }

}

CoordList RandomWalkGenerator::RunRandomWalk()
{
    var currentPosition = m_position;
    CoordList floorPositions;

    for (int i = 0; i < m_iterations; i++)
    {
        // calls function recursively
        CoordList walkedPositions = Algorithms::RandomWalk(currentPosition, m_walkLength);

        for (Coord position : walkedPositions)
        {
            // if the floor is not in the vector, add it
            if (std::find(floorPositions.begin(), floorPositions.end(), position) == floorPositions.end())
                floorPositions.push_back(position);
        }

        //floorPositions.insert(floorPositions.end(), newPositions.begin(), newPositions.end());        

        // starts new path at a random point on the existing one if this option is turned on
        if (m_startRandomly)
            currentPosition = floorPositions[rand() % floorPositions.size()];
    }

    return floorPositions;
}