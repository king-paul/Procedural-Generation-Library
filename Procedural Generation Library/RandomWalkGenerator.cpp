#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

// 
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

// 
void RandomWalkGenerator::Generate()
{
	InitDungeon();    

	CoordList floorPositions = RunRandomWalk(m_position);   
    //AddToDungeon(&floorPositions, 1);

    CoordList wallPositions = Algorithms::GetWalls(floorPositions, {m_width, m_height});
    AddToDungeon(&floorPositions, &wallPositions);
    //DrawMap();
}

// 
CoordList RandomWalkGenerator::RunRandomWalk(Coord startPosition)
{
    var currentPosition = startPosition;
    CoordList floorPositions;

    for (int i = 0; i < m_iterations; i++)
    {
        // calls function recursively
        CoordList walkedPositions = Algorithms::RandomWalk(m_width, m_height, currentPosition, m_walkLength);

        for (Coord position : walkedPositions)
        {
            // if the floor is not in the vector, add it
            if (!CoordInList(&floorPositions, position))
                floorPositions.push_back(position);
        }

        // starts new path at a random point on the existing one if this option is turned on
        if (m_startRandomly)
            currentPosition = floorPositions[rand() % floorPositions.size()];
    }

    return floorPositions;
}