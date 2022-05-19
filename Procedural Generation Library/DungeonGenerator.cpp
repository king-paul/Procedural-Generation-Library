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

	InitMap();
}

void DungeonGenerator::InitMap()
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

void DungeonGenerator::BuildMap(CoordList* floorData, CoordList* wallData)
{
	for (int y = 0; y < m_height; y++)
	{
		m_map.push_back(vector<int>());

		for (int x = 0; x < m_width; x++)
		{
			for (Coord position : *floorData)
			{
				if (position.x == x && position.y == y)
				{
					m_map[y][x] = 1;
					//m_map[y].push_back(1);
					//DrawMap();
					continue;
				}
			}

			for (Coord position : *wallData)
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

void DungeonGenerator::BuildMap(CoordList* data, int type)
{
	for (int y = 0; y < m_height; y++)
	{
		m_map.push_back(vector<int>());

		for (int x = 0; x < m_width; x++)
		{
			for (Coord position : *data)
			{
				if (position.x == x && position.y == y)
				{
					m_map[y][x] = type;
					continue;
				}
			}
		}
	}
}

void DungeonGenerator::DrawMap()
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
				std::cout << "o";
			else if (value == -1)
				std::cout << "*";
		}

		std::cout << std::endl;
	}
}