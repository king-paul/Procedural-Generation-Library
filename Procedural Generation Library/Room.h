#pragma once
#include "Types.h"

class Room
{
private:
	vector<Coord> m_tiles;
	vector<Coord> m_edgeTiles;
	vector<Room*> m_connectedRooms;

	int m_roomSize;
	bool m_accessFromMain;
	bool m_mainRoom;

public:
	Room() { }

	/// <summary>
	/// Instantiates a new room
	/// </summary>
	/// <param name="roomTiles: ">The locations of all tiles within a room</param>
	/// <param name="map: ">The map of the entire cave</param>
	Room(vector<Coord> roomTiles, Array2D<int>* map);

	~Room() { }

	// Getters
	bool IsAccessibleFromMain() { return m_accessFromMain; }
	bool IsMainRoom() { return m_mainRoom; }
	bool HasConnections() { return m_connectedRooms.size() > 0; }
	int GetEdgeCount() { return (int) m_edgeTiles.size(); }
	Coord GetEdgeTile(int index) { return m_edgeTiles[index]; }
	vector<Coord> GetTiles() { return m_tiles; }

	// turns on boolean variables to mark this room as the main one
	void SetAsMainRoom() { 
		m_mainRoom = true; 
		m_accessFromMain = true;
	}

	//vector<Room>* GetConnections() { return &m_connectedRooms; }
	//int GetTotalConnections() { return m_connectedRooms.size(); }

	void ConnectRoom(Room* other);	
	/// <summary>
	/// Checks if another room passed as a parameter is connected to this one
	/// </summary>
	/// <returns>true or false based on whether the two rooms are connected</returns>
	bool IsConnectedTo(Room* otherRoom);
	// Returns the size of another room compared to the current one
	int CompareTo(Room* other);
	// If one room is accesible from the main room
	// set other rooms to be accesible
	void SetAccessible();

	// static functions

	// Joins two rooms together by adding them to the list of connected rooms
	static void ConnectRooms(Room* roomA, Room* roomB);

	// operator overloads
	bool operator ==(const Room* other) const
	{
		return this == other;
	}

	bool operator < (const Room& other) const
	{
		if (m_roomSize < other.m_roomSize)
			return true;
		else
			return false;
	}

	bool operator > (const Room& other) const
	{
		if (m_roomSize > other.m_roomSize)
			return true;
		else
			return false;
	}
	
};