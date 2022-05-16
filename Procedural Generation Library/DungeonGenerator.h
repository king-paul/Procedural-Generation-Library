#pragma once
#include "Algorithms.h"

namespace ProceduralGeneration
{
	class DungeonGenerator
	{
	protected:
		std::vector<int> m_map[2];

		std::vector<int> m_floorCoords[2];
		std::vector<int> m_wallCoords[2];

		int m_size;

		DungeonGenerator(int size)
		{		
			m_size = size;
			//m_map = new int*[m_size];			
		}

		~DungeonGenerator()
		{
			
		}

		void ClearMap()
		{
			//delete[] m_map;
			//m_map = new int*[m_size];
			m_map[0].clear();
			m_map[1].clear();
		}

	public:
		virtual void Generate() = 0; // abstract function		

		std::vector<int>* GetFloorCoords() { return m_floorCoords; }
		std::vector<int>* GetWallCoords() { return m_wallCoords; }

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