#pragma once
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <random>
#include <chrono>
#include <iostream>

#include "Vector.h"

#define Add push_back
#define var auto

// console colors
#define BLACK 30
#define RED 31
#define GREEN 32
#define BROWN 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

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

template <class T>
class Array2D
{
	T* data;

	unsigned int rows = 0;
	unsigned int cols = 0;

public:
	Array2D(unsigned int cols, unsigned int rows)
	{
		data = new T[rows * cols];

		this->rows = rows;
		this->cols = cols;
	}

	Array2D(unsigned int cols, unsigned int rows, T value)
	{
		data = new T[rows * cols];

		this->rows = rows;
		this->cols = cols;

		for (unsigned int i = 0; i < (rows * cols); i++)
		{
			data[i] = value;
		}
	}

	// copy constructor
	Array2D(const Array2D& ref)
	{
		rows = ref.rows;
		cols = ref.cols;

		data = new T[rows * cols];
		
		for (unsigned int i = 0; i < rows * cols; i++)
		{
			data[i] = ref.data[i];
		}

	}

	// assignment operator
	Array2D& operator =(const Array2D& ref)
	{
		if (this == &ref)
		{
			return;
		}

		delete[] data;

		rows = ref.rows;
		cols = ref.cols;

		data = new T[rows * cols];

		for (int i = 0; i < rows * cols; i++)
		{
			data[i] = ref.data[i];
		}
	}

	~Array2D()
	{
		delete[] data;
	}

	T& at(unsigned int col, unsigned int row)
	{
		return data[IndexOf(row, col)];
	}

	T get(unsigned int col, unsigned int row)
	{
		return data[IndexOf(row, col)];
	}

	void set(unsigned int col, unsigned int row, T value)
	{
		data[IndexOf(row, col)] = value;
	}

	int getSize(int dimention)
	{
		if (dimention == 0)
			return rows;
		if (dimention == 1)
			return cols;

		return 0;
	}

	void clear()
	{
		for (int i = 0; i < (int) (rows * cols); i++)
		{
			data[i] = NULL;
		}
	}

private:
	int IndexOf(unsigned int row, unsigned int col)
	{
		return col + row * cols;
	}
};

class PseudoRandom
{
	std::default_random_engine theEngine;
	std::uniform_real_distribution<float> distribution;

public:
	PseudoRandom()
	{
		distribution = std::uniform_real_distribution<float>();
	}

	PseudoRandom(unsigned int seed)
	{
		theEngine = std::default_random_engine(seed);
		distribution = std::uniform_real_distribution<float>();
	}

	PseudoRandom(float min, float max)
	{		
		distribution = std::uniform_real_distribution<float>(min, max);
	}

	PseudoRandom(int min, int max)
	{
		distribution = std::uniform_real_distribution<float>((float) min, (float)max);
	}

	PseudoRandom(float min, float max, unsigned int seed)
	{
		theEngine = std::default_random_engine(seed);
		distribution = std::uniform_real_distribution<float>((float) min, (float) max);
	}

	PseudoRandom(int min, int max, unsigned int seed)
	{
		theEngine = std::default_random_engine(seed);
		distribution = std::uniform_real_distribution<float>((float) min, (float) max);
	}

	void SetRange(float min, float max)
	{
		distribution = std::uniform_real_distribution<float>(min, max);
	}

	void SetRangeInt(int min, int max)
	{
		distribution = std::uniform_real_distribution<float>((float) min, (float) max);
	}

	float GetValue() 
	{
		return distribution(theEngine);
	}

	int GetIntValue()
	{
		return (int) distribution(theEngine);
	}

};

/******************************
 * Dungeon Generation Structs *
 ******************************/
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


	bool operator !=(Coord rhs)
	{
		if (x == rhs.x && y == rhs.y)
			return false;

		return true;
	}

	static float DistanceBetween(Coord a, Coord b)
	{
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


// triangle structure to build mesh with that hold 3 integers for the vertex indicies
struct Triangle
{
	int vertexIndexA;
	int vertexIndexB;
	int vertexIndexC;
	int vertices[3];

	Triangle(int a, int b, int c)
	{
		vertexIndexA = a;
		vertexIndexB = b;
		vertexIndexC = c;

		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;
	}

	// determins what value to return when object is used like an array
	int get(int i) {

		return vertices[i];
	}

	/// <summary>
	/// Checkes whether a vertex is in a tiangle
	/// </summary>
	/// <param name="vertexIndex">The index to check</param>
	/// <returns>return a true or false based on wehter the vertex is found</returns>
	bool Contains(int vertexIndex)
	{
		return vertexIndex == vertexIndexA || vertexIndex == vertexIndexB || vertexIndex == vertexIndexC;
	}
};