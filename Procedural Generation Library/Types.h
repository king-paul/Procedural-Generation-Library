#pragma once
#include <vector>
#include <queue>

#define Add push_back
#define var auto

using namespace std;

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
};

struct Boundary
{
	Coord min;
	Coord max;

	Boundary(Coord min, Coord max)
	{
		this->min = min;
		this->max = max;
	}

	Coord Size()
	{
		return { max.x - min.x, max.y - min.y };
	}
};

typedef std::vector<Coord> CoordList;

class Direction2D
{
public:

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

};