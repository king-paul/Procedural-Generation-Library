#pragma once
#include "Types.h"
#include "Room.h"
#include "Uitls.h"
#include "SquareGrid.h"

namespace ProceduralGeneration
{

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
    bool m_forceAccessToMain; // ensures that all rooms are connected to the main room

    Array2D<int>* m_map; // a series of on and off values of either 1 or 0
    SquareGrid* m_squareGrid; // marching squares build from map

    vector<Room*> m_rooms; // the rooms in the cave to be connected by paths

public:

    // Creates a new instance of the caver generator
    CaveGenerator(int width = 70, int height = 40, int fillPercent = 50, int smoothingIterations = 5, int borderSize = 1,
        int wallThresholdSize = 50, int roomThresholdSize = 50, int passageWidth = 1, bool forceAccessToMain = true, bool useRandomSeed = true, string seed = "");

    ~CaveGenerator() {
        delete m_map; 
        delete m_squareGrid;

        for (Room* room : m_rooms)
            delete room;
        m_rooms.clear();
    }

    /// <summary>
    /// Generates a new random cave from the value passed to the constructor
    /// </summary>    
    void GenerateMap();

    /// <summary>
    /// Gets the generated cave from the cave generator
    /// </summary>
    /// <returns>A 2d array of integer values</returns>
    Array2D<int>* GetMap()
    {
        return m_map;
    }

    SquareGrid* GetSquareGrid() { return m_squareGrid; }

    // Drawing Functions

    // print contents of generated map array to the console
    void PrintCave();
    void PrintCaveWithGrid();      

private:

    // Finds all walls and all rooms in detected regions and removes them from the map
    void ProcessMap();

    /// <summary>
    /// 
    /// </summary>
    Array2D<int> CreateBorderedMap();

    /// <summary>
    /// Gets a list of all of the regions containing a specified tile type
    /// </summary>
    /// <param name="tileType">The type of tile. 0 = vacant, 1 = wall</param>
    /// <returns>2D vector of coordinates for each tile</returns>
    vector<vector<Coord>> GetAllRegions(int tileType);

    /// <summary>
    /// Gets the coordinates of all tiles in a specified region using the flood fill algorithm
    /// </summary>
    /// <param name="startX">The starting coordinate on the x axis</param>
    /// <param name="startY">The starting coordinate on the y axis</param>
    /// <returns>Returns all of the tiles as a list of coordinates</returns>
    vector<Coord> GetRegionCoords(int startX, int startY);

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
    void ConnectClosestRooms(vector<Room*>* allRooms, bool forceAccess = false);

    // Creates floor tiles between two rooms
    void CreatePassage(Room* roomA, Room* roomB, Coord tileA, Coord tileB);

    // calculates a straight line between two coordinate points
    void CreateCircle(Coord coordinate, int radius);

    // creates a vector of all of the regions containing a specified tile type
    std::vector<Coord> CreateLine(Coord from, Coord to);

    // Converts a a grid position to world space
    Vector3 CoordToWorldPoint(Coord tile);

    // printing functions
    void PrintCaveWithGrid(queue<Coord> coords);
    void PrintCaveWithGrid(vector<Coord> allTiles, Coord currenTile, int x, int y);
    void PrintRoomOnMap(vector<Coord> coords);
    void DrawCheckedPositions(Array2D<int>* flags);
    void DrawAtPos(int x, int y, char character, unsigned int color = 0, bool usingGrid = false);
    void HighlightRoom(Room* room, unsigned int color);
};

}