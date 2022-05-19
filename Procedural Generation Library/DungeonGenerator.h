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
		
		// member variables
		std::vector<vector<int>> m_map;
		int m_width, m_height;
		Coord m_startPosition;

		// functions
		DungeonGenerator(int width, int height, Coord startPosition);
		void InitMap();
		void BuildMap(CoordList* floorData, CoordList* wallData);
		void BuildMap(CoordList* data, int type);

	public:
		~DungeonGenerator()	{ }
		virtual void Generate() = 0; // abstract function	
		//int** GetMap() { return m_map; }
		int GetSpaceValue(int x, int y)	{ return m_map[y][x]; }
		void DrawMap();

	};

	// Subclass 1
	class RandomWalkGenerator : public DungeonGenerator
	{
	public:

		RandomWalkGenerator(int width, int height, Coord position, int iterations = 10, int walkLength = 10, bool startRandomly = true);
		~RandomWalkGenerator() {}

		void Generate() override;

		CoordList RunRandomWalk();

		Coord getPosition() { return m_position; }

	protected:
		Coord m_position;
		int m_iterations;
		int m_walkLength;
		bool m_startRandomly;
	};


	// Subclass 2
	class CorridorFirstGenerator : public DungeonGenerator
	{
		int m_corridorLength, m_TotalCorridors;
		float m_roomPercent;

		RandomWalkGenerator* randomWalk;

	public:		
		CorridorFirstGenerator(int dungeonWidth, int dungeonHeight, Coord startPosition = {0, 0},
							   int corridorLength = 30, int totalCorridors = 10, float roomPercent = 0.5f);

		void Generate() override;

	private:
		void CreateRoomsAtDeadEnd(CoordList* deadEnds, CoordList* roomFloors);
		CoordList FindAllDeadEnds(CoordList* floorPositions);
		CoordList CreateRooms(CoordList* potentialRoomPositions);
		void CreateCorridors(CoordList& floorPositions, CoordList& potentialRoomPositions);
	};



	// Subclass 3
	class RoomFirstGenerator : public RandomWalkGenerator
	{
		// member variables
		int minRoomWidth, minRoomHeight;
		int dungeonWidth, dungeonHeight;
		int offset;
		bool randomWalkRooms = false;

	public:
		RoomFirstGenerator(int minRoomWidth, int minRoomHeight, int dungeonWidth, int dungeonHeight, 
						   Coord startPosition, bool randomWalkRooms);

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