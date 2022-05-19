#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

DungeonGenerator::DungeonGenerator(int width, int height, Coord startPosition)
{
	m_width = width;
	m_height = height;
	m_startPosition = startPosition;

	//m_map = new int*[m_size];	

	//for(int i=0; i< size; i++)
		//m_map[i] = new int(0);

	InitDungeon();
}

void DungeonGenerator::InitDungeon()
{
	m_map.clear();

	// build the map with empty spaces
	for (int y = 0; y < m_height; y++)
	{
		m_map.push_back(vector<int>());

		for (int x = 0; x < m_width; x++)
		{
			m_map[y].push_back(0);
		}
	}

}

void DungeonGenerator::AddToDungeon(CoordList* floorData, CoordList* wallData)
{
	for (Coord position : *floorData)
	{
		if (Algorithms::isOutOfBounds(m_width, m_height, position) == false)
			m_map[position.y][position.x] = 1;
	}

	for (Coord position : *wallData)
	{
		if (Algorithms::isOutOfBounds(m_width, m_height, position) == false)
			m_map[position.y][position.x] = -1;
	}
}

void DungeonGenerator::AddToDungeon(CoordList* data, int type)
{
	for (Coord position : *data)
	{
		if (Algorithms::isOutOfBounds(m_width, m_height, position) == false)
		{
			//std::cout << "X: " << position.x << ", Y: " << position.y << std::endl;
			m_map[position.x][position.y] = type;
		}

	}

}

void DungeonGenerator::DrawDungeon()
{
	system("cls");

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int value = GetSpaceValue(x, y);

			if (value == 0)
				std::cout << ".";
			else if (value == 1)
				std::cout << " ";
			else if (value == -1)
				std::cout << "*";
		}

		std::cout << std::endl;
	}
}