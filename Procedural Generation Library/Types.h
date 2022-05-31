#pragma once
#include <vector>
#include <queue>
#include <string>
#include <map>

#include "Vector.h"

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

template <class T>
class Array2D
{
	T* data;

	unsigned int rows = 0;
	unsigned int cols = 0;

public:
	Array2D(unsigned int cols, unsigned int row)
	{
		data = new T[rows * cols];
		
		/*
		for (int i = 0; i < rows * cols; i++)
		{
			data[i] = nullptr;
		}*/

		this->rows = rows;
		this->cols = cols;
	}

	// copy constructor
	Array2D(const Array2D& ref)
	{
		rows = ref.rows;
		cols = ref.cols;

		data = new T[rows * cols];
		
		for (int i = 0; i < rows * cols; i++)
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

	T& get(unsigned int col, unsigned int row)
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

private:
	int IndexOf(unsigned int row, unsigned int col)
	{
		return col + row * cols;
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

/*********************
 * Square Gird types *
 *********************/
struct Node
{
	Vector3 position;
	int vertexIndex;

	Node() {
		position = Vector3::Zero();
		vertexIndex = -1;
	}

	Node(Vector3 pos)
	{
		position = pos;
		vertexIndex = -1;
	}
};

struct ControlNode : Node
{
	bool active;
	Node *above, *right; // The positions above and to the right of the control node

	
	ControlNode()
	{
		active = false;
		above = nullptr;
		right = nullptr;
	}

	ControlNode(Vector3 pos, bool active, float squareSize) : Node(pos), active(active)
	{
		Vector3 forwardVector = Vector3::Forward() * ((float)squareSize / 2.0f);
		Vector3 rightVector = Vector3::Forward() * ((float) squareSize / 2.0f);

		// set the position to the distance above
		above = new Node(position + forwardVector);
		// set the position to the distance to the right
		right = new Node(position + rightVector);
	}

	~ControlNode()
	{
		delete above;
		delete right;
	}

};

struct Square
{
	ControlNode *topLeft, *topRight, *bottomRight, *bottomLeft;
	Node *centreTop, *centreRight, *centreBottom, *centreLeft;

	short configuration; // value is between 0000 and 1111
	
	Square()
	{
		topLeft = nullptr;
		topRight = nullptr;
		bottomLeft = nullptr;
		bottomRight = nullptr;

		centreTop = nullptr;
		centreRight = nullptr;
		centreBottom = nullptr;
		centreLeft = nullptr;

		configuration = 0000;
	}

	Square(ControlNode* topLeft, ControlNode* topRight, ControlNode* bottomRight, ControlNode* bottomLeft)
	{
		// set the cornor nodes
		this->topLeft = topLeft;
		this->topRight = topRight;
		this->bottomRight = bottomRight;
		this->bottomLeft = bottomLeft;

		// set center nodes to the nodes connected to the corner nodes
		this->centreTop = topLeft->right;
		this->centreRight = bottomRight->above;
		this->centreBottom = bottomLeft->right;
		this->centreLeft = bottomLeft->above;

		configuration = 0;

		// turns on the appropriate bit in the four bit number base on
		// which control nodes are active
		if (topLeft->active)
			configuration += 8; // turns on the first bit
		if (topRight->active)
			configuration += 4; // turns on the second bit
		if (bottomRight->active)
			configuration += 2; // turns on the thrid bit
		if (bottomLeft->active)
			configuration += 1; // turns on the fourth bit			
	}
};

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