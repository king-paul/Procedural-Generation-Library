#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

RandomWalkDungeonGenerator::RandomWalkDungeonGenerator(int size, Coord position, int iterations, int walkLength, bool startRandomly) : 
	DungeonGenerator(size)
{
	m_position = position;
	m_iterations = iterations;
	m_walkLength = walkLength;
	m_startRandomly = startRandomly;
}

void RandomWalkDungeonGenerator::Generate()
{
	ClearMap();
	CoordList floorPositions = RunRandomWalk();
    CoordList wallPositions = Algorithms::GetWalls(floorPositions);


    for (int y = 0; y < m_size; y++)
    {
        for (int x = 0; x < m_size; x++)
        {
            for (Coord position : floorPositions)
            {
                if (position.x == x && position.y == y)
                {
                    m_floorCoords[0].Add(x);
                    m_floorCoords[1].Add(y);
                    continue;
                }
            }
            
            for (Coord position : wallPositions)
            {
                if (position.x == x && position.y == y)
                {
                    m_wallCoords[0].Add(x);
                    m_wallCoords[1].Add(y);
                    continue;
                }
            }
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
        floorPositions = Algorithms::RandomWalk(currentPosition, m_walkLength);

        //floorPositions.push_back(); // joins two hashsets together

        // starts new path at a random point on the existing one if this option is turned on
        if (m_startRandomly)
            currentPosition = floorPositions[rand() % floorPositions.size()];
    }

    return floorPositions;
}