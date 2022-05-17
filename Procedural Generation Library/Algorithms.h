#pragma once
#include "Types.h"

namespace ProceduralGeneration
{
	class Algorithms
	{
		static CoordList m_path;

	public:

		/// <summary>
		/// Algorithm that walks in a random direction a set number of times
		/// </summary>
		/// <param name="startPosition">The starting coordinate the algorithm will run from</param>
		/// <param name="walkLength">How many steps will be walked before stopping</param>
		/// <returns>Generated path</returns>
		static CoordList RandomWalk(Coord startPosition, int walkLength);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="starPosition"></param>
		/// <param name="corridorLength"></param>
		/// <returns></returns>
		static CoordList RandomWalkCorridor(Coord starPosition, int corridorLength);

		/// <summary>
		/// Splits a section of the dungeon into smaller sections using the binary space partitioning algorithm
		/// </summary>
		/// <param name="spaceToSplit">The bounding box to be split into two boxes</param>
		/// <param name="minWidth">The minimum width of the rooms to create</param>
		/// <param name="minHeight">The minimum height of the rooms to create</param>
		/// <returns>A list of bounding boxes using Boundary struct</returns>
		static vector<Boundary> BinarySpacePartitioning(Boundary spaceToSplit, int minWidth, int minHeight);
		
		/// <summary>
		/// Creates wall tiles using the tilemap visualizer by passing the floor positions
		/// </summary>
		/// <returns>A list of coordinates for the will tiles</returns>
		static CoordList GetWalls(CoordList floorPositions);

	private:

		/// <summary>
	    /// splits a room down the middle into two rooms to the left and the right
	    /// </summary>
	    /// <param name="minWidth">Not currently used</param>
	    /// <param name="roomsQueue">The queue of bounding boxes to be split</param>
	    /// <param name="room">The room to be split</param>
		static void SplitVertically(int minWidth, queue<Boundary> roomsQueue, Boundary room);

		/// <summary>
		///  splits a rooom across the middle into two rooms above and below
		/// </summary>
		/// <param name="minHeight">Not currently used</param>
		/// <param name="roomsQueue">The queue of bounding boxes to be split</param>
		/// <param name="room">The room to be split</param>
		static void SplitHorizontally(int minHeight, queue<Boundary> roomsQueue, Boundary room);

		// Find all adjacent walls in specified directions
		static CoordList FindWallsInDirection(CoordList floorPositions, CoordList directionList);
	};

}