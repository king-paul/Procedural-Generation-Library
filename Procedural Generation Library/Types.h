#pragma once
#include <vector>
#include <queue>
#include <string>
#include <map>

#define Add push_back
#define var auto

using namespace std;

enum class TileType
{
	Empty, // 0
	Floor, // 1
	WallFull, // 2
	Top, // 3
	Bottom, // 4
	SideLeft, // 5
	SideRight, // 6
	SideBottom, // 7
	InnerCornerDownLeft, // 8
	InnerCornerDownRight, // 9
	DiagonalCornerDownLeft, // 10
	DiagonalCornerDownRight, // 11
	DiagonalCornerUpLeft, // 12
	DiagonalCornerUpRight // 13
};

/***********
 * Structs *
 ***********/
template <class T>
struct Array2D
{
	T** data;

	T get(int row, int col) {
		data[row][col];
	}

	void set(int row, int col, T value)
	{
		data[row][col] = value;
	}

	int Length() 
	{
		return 0;
	}

};

template <class T>
struct JaggedArray : public Array2D<T>
{

	JaggedArray(int rows, int cols)
	{

	}

};

struct Coord
{
	int x;
	int y;

	Coord()
	{
		x = 0;
		y = 0;
	}

	Coord(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Coord operator +(Coord rhs)
	{
		return { x + rhs.x, y + rhs.y };
	}

	Coord operator -(Coord rhs)
	{
		return { x - rhs.x, y - rhs.y };
	}

	Coord operator *(int rhs)
	{
		return { x * rhs, y * rhs };
	}

	void operator +=(Coord rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	void operator -=(Coord rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	bool operator ==(Coord rhs)
	{
		if (x == rhs.x && y == rhs.y)
			return true;

		return false;
	}

	static float DistanceBetween(Coord a, Coord b)
	{
		/*
		Coord difference;
		float aMagnitude = sqrt(a.x * a.x + a.y * a.y);
		float bMagnitude = sqrt(b.x * b.x + b.y * b.y);

		if (aMagnitude >= bMagnitude)
			difference = a - b;
		else
			difference = b - a;

		return sqrt(difference.x * difference.x + difference.y * difference.y);*/

		float num = (float) (a.x - b.x);
		float num2 = (float) (a.y - b.y);
		return (float)sqrt(num * num + num2 * num2);
	}
};

struct Boundary
{
	Coord min;
	Coord max;

	Coord size;

	Boundary(Coord min, Coord size)
	{
		this->min = min;		
		this->size = size;
		this->max = min + size;
	}

	Coord Size()
	{
		return size;
	}

	int GetWidth() { return max.x - min.x; }
	int GetHeight() { return max.y - min.y; }

	Coord Center()
	{
		int width = max.x - min.x;
		int height = max.y - min.y;

		int x = min.x + (width / 2);
		int y = min.y + (height / 2);

		return Coord(x, y);
	}

};

struct Wall
{
	Wall(Coord position, TileType tile) { 
		this->position = position; 
		this->tile = tile;
	}

	Coord position;
	TileType tile;
};

typedef std::vector<Coord> CoordList;

static bool CoordInList(CoordList* list, Coord position)
{
	CoordList::iterator iterator = std::find(list->begin(), list->end(), position);

	if (iterator != list->end())
		return true;
		
	return false;
}

enum class Direction
{
	Up, Down, Left, Right
};

// classes
class Direction2D
{
public:

	static Coord Up() { return { 0, 1 }; }
	static Coord Down() { return { 0, -1 }; }
	static Coord Left() { return { -1, 0 }; }
	static Coord Right() { return { 1, 0 }; }

	static CoordList CardinalDirections()
	{
		return {
			{0, 1 }, // UP
			{1, 0 }, // RIGHT
			{0, -1}, // DOWN
			{-1, 0} //LEFT
		};
	}

	static CoordList DiagonalDirections()
	{
		return {
			{1, 1}, // UP-RIGHT
			{1, -1}, // RIGHT-DOWN
			{-1, -1}, // DOWN-LEFT
			{-1, 1} // LEFT-UP
		};
	}

	static CoordList AllDirections()
	{
		return {
			{0, 1 }, // UP
			{1, 1}, // UP-RIGHT
			{1, 0 }, // RIGHT
			{1, -1}, // RIGHT-DOWN
			{0, -1}, // DOWN						
			{-1, -1}, // DOWN-LEFT
			{-1, 0}, //LEFT	
			{-1, 1} // LEFT-UP
		};
	}

	static Coord GetRandomCardinalDirection()
	{
		CoordList cardinalDirections = CardinalDirections();

		return cardinalDirections[rand() % cardinalDirections.size()];
	}

	static Coord GetRandomTurnDirection(Coord direction)
	{		
		Coord possibleDirections[2];

		// Vertical directions
		if (direction == Coord(0, 1) || direction == Coord(0, -1)) { 

			// Left and right
			possibleDirections[0] = { -1, 0 };
			possibleDirections[1] = {1, 0}; 
		}

		// Horizontal directions
		else if (direction == Coord(-1, 0) || direction == Coord(1, 0))
		{
			// Up and down
			possibleDirections[0] = { 0, 1 };
			possibleDirections[1] = { 0, -1 };
		}

		// randomly select one of the two possible directions and return the value
		return possibleDirections[rand() % 2];
	}

	static Direction GetOpposite(Direction direction)
	{
		switch (direction)
		{
			case Direction::Up:
				return Direction::Down;
			case Direction::Down:
				return Direction::Up;
			case Direction::Left:
				return Direction::Right;
			case Direction::Right:
				return Direction::Left;
			default:
				return direction;
		}
	}
		

};