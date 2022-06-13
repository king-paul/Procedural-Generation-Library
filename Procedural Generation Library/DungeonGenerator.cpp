#include "DungeonGenerator.h"

using namespace ProceduralGeneration;

DungeonGenerator::DungeonGenerator(int width, int height, Coord startPosition)
{
	m_width = width;
	m_height = height;
	m_startPosition = startPosition;

	srand(time(NULL));

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
		m_map.push_back(vector<TileType>());

		for (int x = 0; x < m_width; x++)
		{
			m_map[y].push_back(TileType::Empty);
		}
	}

}

void DungeonGenerator::AddToDungeon(CoordList* floorData, vector<Wall>* wallData)
{
	for (Coord position : *floorData)
	{
		if (Algorithms::isOutOfBounds(m_width, m_height, position) == false)
			m_map[position.y][position.x] = TileType::Floor;
	}

	for (Wall wall : *wallData)
	{
		if (Algorithms::isOutOfBounds(m_width, m_height, wall.position) == false)
		{
			m_map[wall.position.y][wall.position.x] = wall.tile;
		}			
	}
}

void DungeonGenerator::AddToDungeon(CoordList* data, TileType type)
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
			//TileType tile = GetSpaceValue(x, y);
			//std::cout << (int)tile;
			
			/*
			if (tile == TileType::Empty)
				std::cout << ".";
			else if (tile == TileType::Floor)
				std::cout << "O";
			else
				std::cout << "*";*/

			switch ((int)GetSpaceValue(x, y))
			{
				case 0:	cout << " ";
					break;

				case 1:	cout << " ";
					break;

				case 2:	cout << "*";
					break;

				case 3: cout << "_";
					break;
				
				case 4: case 7:	cout << "-";
					break;

				case 5: case 6:	cout << "|";
					break;

				case 8:	cout << "<";
					break;

				case 9: cout << ">";
					break;

				case 10: case 13:
					cout << "/";
					break;

				case 11: case 12:
					cout << "\\";
					break;
			}
		}

		cout << endl;
	}
}