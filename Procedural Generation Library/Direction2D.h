#pragma once

#include "Types.h"

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

	static Coord GetRandomCardinalDirection(PseudoRandom& generator)
	{
		CoordList cardinalDirections = CardinalDirections();
		generator.SetRangeInt(0, (int) cardinalDirections.size());

		return cardinalDirections[generator.GetIntValue()];
	}

	static Coord GetRandomTurnDirection(Coord direction)
	{
		Coord possibleDirections[2];

		// Vertical directions
		if (direction == Coord(0, 1) || direction == Coord(0, -1)) {

			// Left and right
			possibleDirections[0] = { -1, 0 };
			possibleDirections[1] = { 1, 0 };
		}

		// Horizontal directions
		else if (direction == Coord(-1, 0) || direction == Coord(1, 0))
		{
			// Up and down
			possibleDirections[0] = { 0, 1 };
			possibleDirections[1] = { 0, -1 };
		}

		PseudoRandom randomInt(0, 1);

		// randomly select one of the two possible directions and return the value
		return possibleDirections[(int) randomInt.GetValue()];
	}

};