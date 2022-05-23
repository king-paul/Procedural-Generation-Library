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
		std::vector<vector<TileType>> m_map;
		int m_width, m_height;
		Coord m_startPosition;

		// functions
		DungeonGenerator(int width, int height, Coord startPosition);
		void InitDungeon();
		void AddToDungeon(CoordList* floorData, std::vector<Wall>* wallData);
		void AddToDungeon(CoordList* data, TileType type);

	public:
		~DungeonGenerator()	{ }
		virtual void Generate() = 0; // abstract function	
		//int** GetMap() { return m_map; }
		TileType GetSpaceValue(int x, int y) { return m_map[y][x]; }
		void DrawDungeon();

	};

	struct RandomWalkParameters
	{
		RandomWalkParameters(int iterations, int walkLength, bool startRandomly)
		{
			this->iterations = iterations;
			this->walkLength = walkLength;
			this->startRandomly = startRandomly;
		}

		int iterations;
		int walkLength;
		bool startRandomly;
	};

	// Subclass 1
	class RandomWalkGenerator : public DungeonGenerator
	{
	public:

		RandomWalkGenerator(int width, int height, Coord position, int iterations = 10, int walkLength = 10, bool startRandomly = true);
		~RandomWalkGenerator() {}

		virtual void Generate() override;

		CoordList RunRandomWalk(Coord startPosition);

	protected:
		Coord m_position;
		int m_iterations;
		int m_walkLength;
		bool m_startRandomly;
	};


	// Subclass 2
	class CorridorFirstGenerator : public RandomWalkGenerator
	{
		int m_corridorLength, m_TotalCorridors;
		float m_roomPercent;		

	public:		
		CorridorFirstGenerator(int dungeonWidth, int dungeonHeight, Coord startPosition = {0, 0},
							   int corridorLength = 30, int totalCorridors = 10, float roomPercent = 0.5f,
							   RandomWalkParameters parameters = { 10, 10, true});

		void Generate() override;

	private:
		void CreateRoomsAtDeadEnd(CoordList* deadEnds, CoordList& roomFloors);
		CoordList FindAllDeadEnds(CoordList* floorPositions);
		CoordList CreateRooms(CoordList* potentialRoomPositions);
		void CreateCorridors(CoordList& floorPositions, CoordList& potentialRoomPositions);
	};


	// Subclass 3
	class RoomFirstGenerator : public RandomWalkGenerator
	{
		// member variables
		int m_minRoomWidth, m_minRoomHeight;
		int m_offset;
		bool m_randomWalkRooms = false;

	public:
		RoomFirstGenerator(int dungeonWidth, int dungeonHeight,	Coord startPosition,
			int minRoomWidth = 4, int minRoomHeight = 4, int offset = 1, bool randomWalkRooms = false, 
			RandomWalkParameters paramater = { 0, 0, 0});

		void Generate() override;

	private:
		CoordList ConnectRooms(CoordList roomCenters);
		CoordList CreateCorridor(Coord currentRoomCenter, Coord destination);
		Coord FindClosestPointTo(Coord currentRoomCenter, CoordList roomCenters);
		CoordList CreateSimpleRooms(std::vector<Boundary> rooms);
		CoordList CreateRandomWalkRooms(std::vector<Boundary> roomsList);
	};
}

}