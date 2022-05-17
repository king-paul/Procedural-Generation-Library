#pragma once
#include "Algorithms.h"
#include <time.h>
#include <iostream>

#define MAX_COLS 100

namespace ProceduralGeneration
{

extern "C" {

	class DungeonGenerator
	{
	protected:
		//int** m_map;
		std::vector<vector<int>> m_map;

		//std::vector<int> m_floorCoords[2];
		//std::vector<int> m_wallCoords[2];

		int m_size;

		DungeonGenerator(int size)
		{		

			m_size = size;
			//m_map = new int*[m_size];	

			//for(int i=0; i< size; i++)
				//m_map[i] = new int(0);
			InitMap();
			
		}

		~DungeonGenerator()
		{

		}

		void InitMap()
		{
			//delete[] m_map;
			//m_map = new int*[m_size];
			//m_map[0].clear();
			//m_map[1].clear();
			m_map.clear();

			// build the map with empty spaces
			for (int y = 0; y < m_size; y++)
			{
				m_map.push_back(vector<int>());

				for (int x = 0; x < m_size; x++)
				{
					m_map[y].push_back(0);
				}
			}
		}

	public:
		virtual void Generate() = 0; // abstract function		

		//int* GetFloorCoords() { return m_floorCoords->data(); }
		//int* GetWallCoords() { return m_wallCoords->data(); }
		//int** GetMap() { return m_map; }

		int GetSpaceValue(int x, int y)
		{
			return m_map[y][x];
		}

		void DrawMap()
		{
			system("cls");

			for (int y = 0; y < m_size; y++)
			{
				for (int x = 0; x < m_size; x++)
				{
					int value = GetSpaceValue(x, y);

					if (value == 0)
						std::cout << " ";
					else if (value == 1)
						std::cout << "o";
					else if (value == -1)
						std::cout << "*";
				}

				std::cout << std::endl;
			}
		}

	};

	// Subclass 1
	class RandomWalkDungeonGenerator : public DungeonGenerator
	{
	public:

		RandomWalkDungeonGenerator(int size, Coord position, int iterations = 10, int walkLength = 10, bool startRandomly = true);

		void Generate() override;

		CoordList RunRandomWalk();

	protected:
		Coord m_position;
		int m_iterations;
		int m_walkLength;
		bool m_startRandomly;
	};


	// Subclass 2
	class CorridorFirstGenerator : public RandomWalkDungeonGenerator
	{

	public:
		void Generate() override;

	private:
		void CreateRoomsAtdeadEnd(CoordList deadEnds, CoordList roomFloors);
		void FindAllDeadEnds(CoordList floorPositions);
		void CreateRooms(CoordList potentialRoomPositions);
		void CreateCorridors(CoordList floorPositions, CoordList potentialRoomPositions);
	};

	// Subclass 3
	class RoomFirstDungeonGenerator
	{
	public:
		void CreateRooms();

	private:
		CoordList ConnectRooms();
		CoordList CreateCorridor(CoordList roomCenters);
		CoordList FindClosestPointTo(CoordList currentRoomCenter, CoordList roomCenters);
		CoordList CreateSimpleRooms(CoordList roomsList);
		CoordList CreateRoomsRandomly(CoordList roomsList);
	};
}

}