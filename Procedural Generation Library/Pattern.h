#pragma once
#include <string>
#include "Types.h"

class Pattern
{
	int m_index;
	Array2D<int>* m_grid;
	std::string m_hashCode;

public:
	// constructor
	Pattern(Array2D<int>* grid, std::string hashcode, int index) :
		m_grid(grid), m_hashCode(hashcode), m_index(index)
	{

	}

	// getters
	std::string getHashCode() { return m_hashCode; }
	int GetIndex() { return m_index; }

	// other functions

	// Sets value in a grid at a specific position using parameters
	void SetValue(int x, int y, int value);
	// Returns a value from the grid at a specified position
	int GetValue(int x, int y);
	// Checks if value at specified location matches the value passed as a parameter
	bool ValueEqualAtPosition(int x, int y, int value);
	// Checks if the patterns in two opposite directions match each other
	bool EqualToPattern(Direction direction, Pattern* pattern);

private:
	// Creates a new jagged arrayand of the correct sizeand uses
	// it to select the correct part of the value grid
	Array2D<int> GetValuesInDirection(Direction direction);
	// Creates a new grid based on a part to compare
	void CreateSection(int xMin, int xMax, int yMin, int yMax, Array2D<int> gridPartToCompare);
};