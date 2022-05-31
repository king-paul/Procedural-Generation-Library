#pragma once
#include "Types.h"
#include "Room.h"
#include "Uitls.h"

class CaveGenerator
{
    // variable declartaion
    int m_width;
    int m_height;

    string m_seed;
    bool m_useRandomSeed;

    int m_randomFillPercent;
    int m_smoothingIterations;

    int m_borderSize; // the size of the boarder around the map
    int m_wallThresholdSize; // the minimum size of a wall
    int m_roomThresholdSize; // the minimum size of a room

    int m_passageWidth; // how wide the passages connecting rooms is

    Array2D<int>* m_map;

public:
   
    // Creates a new instance of the caver generator
    CaveGenerator(int width = 70, int height = 40, int fillPercent = 50, int smoothingIterations = 5, int borderSize = 1,
        int wallThresholdSize = 50, int roomThresholdSize = 50, int passageWidth = 1, bool useRandomSeed = true, string seed = "");

    ~CaveGenerator() { delete m_map; }
    
    /// <summary>
    /// Generates a new random cave from the value passed to the constructor
    /// </summary>
    /// <returns>A 2d array of integer values</returns>
    Array2D<int>* GenerateMap();

private:

    // Finds all walls and all rooms in detected regions and removes them from the map
    void ProcessMap();

    /// <summary>
    /// Gets a list of all of the regions containing a specified tile type
    /// </summary>
    /// <param name="tileType">The type of tile. 0 = vacant, 1 = wall</param>
    /// <returns></returns>
    vector<vector<Coord>> GetRegions(int tileType);

    /// <summary>
    /// Gets the coordinates of all tiles in a specified region using the flood fill algorithm
    /// </summary>
    /// <param name="startX">The starting coordinate on the x axis</param>
    /// <param name="startY">The starting coordinate on the y axis</param>
    /// <returns>Returns all of the tiles as a list of coordinates</returns>
    vector<Coord> GetRegionTiles(int startX, int startY);

    /// <summary>
    /// Checkes if specified coordinates are in the range of the map and
    /// </summary>
    /// <returns>true or false base of whether they are</returns>
    bool IsInMapRange(int x, int y);

    // Creates a new map at random using a cellular automata algorithm
    void RandomFillMap();

    // Runs through smoothing iterations to smooth out the map
    void SmoothMap();

    /// <summary>
    /// Checks how many tiles around the current one are walls
    /// </summary>
    /// <param name="gridX">The column on the grid</param>
    /// <param name="gridY">The row on the grid</param>
    /// <returns>The total number of walls sorrounding the gird space</returns>
    int GetSurroundingWallCount(int gridX, int gridY);

    // Connects all rooms in the cave that are closest distance from each other
    void ConnectClosestRooms(vector<Room*> *allRooms, bool forceAccessFromMainRoom = false);

    // Creates floor tiles between two rooms
    void CreatePassage(Room* roomA, Room* roomB, Coord tileA, Coord tileB);

    // calculates a straight line between two coordinate points
    void CreateCircle(Coord coordinate, int radius);

    // creates a vector of all of the regions containing a specified tile type
    std::vector<Coord> CreateLine(Coord from, Coord to);

    // Converts a a grid position to world space
    Vector3 CoordToWorldPoint(Coord tile);
};