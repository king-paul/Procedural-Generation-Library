#pragma once
#include <vector>
#include <unordered_set>
#include "Types.h"

// return whether value is positive or negative
template <typename T> int sign(T val) {

	if (val < 0)
		return -1;
	else
		return 1;

	//return (T(0) < val) - (val < T(0));
}

//
template<class T>
bool IsInSet(std::vector<T>* vector, T value)
{
	return std::find(vector->begin(), vector->end(), value) != vector->end();
}

template<class T>
bool IsInSet(std::unordered_set<T>* vector, T value)
{
	return std::find(vector->begin(), vector->end(), value) != vector->end();
}

class Helpers
{
public:
	static bool CoordsInList(vector<Coord> list, int x, int y)
	{
		for (Coord coord : list)
		{
			if (coord.x == x && coord.y == y)
			{
				return true;
			}
		}

		return false;
	}

};