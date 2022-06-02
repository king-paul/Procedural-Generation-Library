#include "Room.h"

Room::Room(vector<Coord> roomTiles, Array2D<int>* map)
{
	m_tiles = roomTiles;
	m_roomSize = (int) m_tiles.size();
	//m_connectedRooms = new List<Room>();

	//m_edgeTiles = new List<Coord>();

	// iterates through all tiles in the map
	for(Coord tile : m_tiles)
	{
		// iterates through the tiles on the left and right of the current tile
		for (int x = tile.x - 1; x <= tile.x + 1; x++)
		{
			// iterates through the tiles on the above and below the current tile
			for (int y = tile.y - 1; y <= tile.y + 1; y++)
			{
				if (x == tile.x || y == tile.y) // if on current tile
				{
					// if the tile is a wall tile add it to the list of edges
					if (map->get(x, y) == 1)
						m_edgeTiles.Add(tile);
				}
			}
		}
	}
}

void Room::ConnectRoom(Room* other)
{
	if (IsAccessibleFromMain())
	{
		m_connectedRooms.Add(other);
	}
	else if (other->IsAccessibleFromMain())
		other->SetAccessible();

	m_connectedRooms.Add(other);
	other->m_connectedRooms.Add(this);
}

void Room::ConnectRooms(Room* roomA, Room* roomB)
{
	if (roomA->IsAccessibleFromMain())
	{
		roomB->m_connectedRooms.Add(roomA);
	}
	else if (roomB->IsAccessibleFromMain())
		roomA->SetAccessible();

	roomA->m_connectedRooms.Add(roomB);
	roomB->m_connectedRooms.Add(roomA);
}

bool Room::IsConnectedTo(Room* otherRoom)
{
	if(std::find(m_connectedRooms.begin(), m_connectedRooms.end(), otherRoom)
		!= m_connectedRooms.end() )
		return true;

	return false;
}

int Room::CompareTo(Room* other)
{
	return other->m_roomSize - m_roomSize;
}

void Room::SetAccessible()
{
	if (!m_accessFromMain)
	{
		m_accessFromMain = true;

		for(Room* connectedRoom : m_connectedRooms)
		{
			connectedRoom->SetAccessible();
		}
	}
	
}