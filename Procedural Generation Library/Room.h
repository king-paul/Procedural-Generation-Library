#pragma once
#include "Types.h"

class Room
{
	vector<Coord> m_tiles;
	vector<Coord> m_edgeTiles;
	vector<Room> m_connectedRoms;

	int m_roomSize;
	bool m_accessFromMain;
	bool m_mainRoom;

public:
	//Room() { }

	/// <summary>
	/// Instantiates a new room
	/// </summary>
	/// <param name="roomTiles: ">The locations of all tiles within a room</param>
	/// <param name="map: ">The map of the entire cave</param>
	Room(vector<Coord> roomTiles, Array2D<int>* map);

	void ConnectRoom(Room other);	
	/// <summary>
	/// Checks if another room passed as a parameter is connected to this one
	/// </summary>
	/// <returns>true or false based on whether the two rooms are connected</returns>
	bool IsConnected(Room otherRoom);
	// Returns the size of another room compared to the current one
	int CompareTo(Room other);
	// If one room is accesible from the main room
	// set other rooms to be accesible
	void SetAccessible();

	// static functions

	// Joins two rooms together by adding them to the list of connected rooms
	static void ConnectRooms(Room roomA, Room roomB);
	
};