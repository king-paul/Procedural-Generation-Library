#include "Algorithms.h"

using namespace ProceduralGeneration;

CoordList Algorithms::RandomWalk(int dungeonWidth, int dungeonHeight, Coord startPosition, int walkLength, PseudoRandom& generator)
{
	CoordList path;	

	path.Add(startPosition);
	var previousPosition = startPosition;
	var newPosition = previousPosition;

	// for each step in the walk
	for (int i = 0; i < walkLength; i++)
	{
		// adds a random position to the pevious position then add sit to the path
		// then moves to the new position provided that it is not outofBounds
		newPosition = previousPosition + Direction2D::GetRandomCardinalDirection(generator);		

		if (isOutOfBounds(dungeonWidth, dungeonHeight, newPosition))
			continue;

		path.Add(newPosition);
		previousPosition = newPosition;
	}

	return path;
}


CoordList Algorithms::RandomWalkCorridor(int dungeonWidth, int dungeonHeight, Coord startPosition, int corridorLength, PseudoRandom& generator)
{
	CoordList corridor;
	Coord direction;

	var currentPosition = startPosition;
	corridor.Add(currentPosition);	

	// pick a random direction and if it will lead out of bounds pick another one
	do {
		direction = Direction2D::GetRandomCardinalDirection(generator);
		//direction = Direction2D::GetRandomTurnDirection(m_lastDirection);
	} while (isOutOfBounds(dungeonWidth, dungeonHeight, startPosition + (direction * corridorLength)));

	for (int i = 0; i < corridorLength; i++)
	{
		// if we are out of bounds go back one step and change direction
		// currently doesn't take corners or spaces already visited into account
		if (isOutOfBounds(dungeonWidth, dungeonHeight, currentPosition + direction))
		{
			direction = Direction2D::GetRandomTurnDirection(direction);
		}

		currentPosition += direction;
		corridor.Add(currentPosition);
	}

	//Algorithms::m_lastDirection = direction; // records current direction for next corridor
	return corridor;
}

vector<Boundary> Algorithms::BinarySpacePartitioning(Boundary spaceToSplit, int minWidth, int minHeight, PseudoRandom& generator)
{
	queue<Boundary> roomsQueue;
	vector<Boundary> roomsList;

	roomsQueue.push(spaceToSplit); // remove the item from the front of the queue

	//cout << "Min Width: " << minWidth << ", Min Height: " << minHeight << endl;

	generator.SetRange(0.0f, 1.0f);

	while (roomsQueue.size() > 0)
	{
		var room = roomsQueue.front();
		roomsQueue.pop();

		// check that the space to split is a minumum size
		if (room.Size().y >= minHeight && room.Size().x >= minWidth)
		{
			//cout << "Room Size x:" << room.Size().x << " y:" << room.Size().y;
			//cout << "    Random value: " << value << endl;			

			// if the random value is less than 0 split the room horizontally
			if (generator.GetValue() < 0.5f)
			{
				// checks if the room can be split horizontally
				// by seeing if it is atleast double the minumum height
				if (room.Size().y >= minHeight * 2)
				{
					SplitHorizontally(minHeight, roomsQueue, room, generator);
				}
				// if the room cannont be split horizontally check if it can be split vertically
				// by seeing if it is atleast double the minumum width
				else if (room.Size().x >= minWidth * 2)
				{
					SplitVertically(minWidth, roomsQueue, room, generator);
				}
				// otherwise  if the room is still above the minumum width and minimum height
				// simply add the room
				else if (room.Size().x >= minWidth && room.Size().y >= minHeight)
				{
					roomsList.Add(room);
				}
			}
			else // if the random value is >= 0.5 split the room vertically
			{
				// checks if the room can be split horizontally
				// by seeing if it is atleast double the minumum width
				if (room.Size().x >= minWidth * 2)
				{
					SplitVertically(minHeight, roomsQueue, room, generator);
				}
				// if the room cannont be split vertically check if it can be split horizontally
				// by seeing if it is atleast double the minumum height
				else if (room.Size().y >= minHeight * 2)
				{
					SplitHorizontally(minWidth, roomsQueue, room, generator);
				}
				// otherwise  if the room is still above the minumum width and minimum height
				//simply add the room
				else if (room.Size().x >= minWidth && room.Size().y >= minHeight)
				{
					roomsList.Add(room);
				}
			}
		}
	}

	return roomsList;
}

void ProceduralGeneration::Algorithms::SplitVertically(int minWidth, queue<Boundary> &roomsQueue, Boundary room, PseudoRandom& generator)
{
	// generate a random number between 1 and the width of the room
	generator.SetRange(1, (float) room.Size().x);
	int xSplit = generator.GetIntValue();

	//cout << "Room Width: " << room.Size().x <<", xSplit: " << xSplit << endl;

	// creates the first room by passing in the start position and the size
	Boundary room1(room.min, Coord(xSplit, room.Size().y));
	// creates the second room by passing in the position of the split and the size
	Boundary room2(Coord(room.min.x + xSplit, room.min.y),
		Coord(room.Size().x - xSplit, room.Size().y));

	// adds the two rooms to the queue that was passed in
	roomsQueue.push(room1);
	roomsQueue.push(room2);
}

void Algorithms::SplitHorizontally(int minHeight, queue<Boundary> &roomsQueue, Boundary room, PseudoRandom& generator)
{
	generator.SetRange(1, (float) room.Size().y);

	// generate a random number between 1 and the height of the room
	int ySplit = generator.GetIntValue();
	//cout << "Room Height: " << room.Size().y << ", ySplit: " << ySplit << endl;

	Boundary room1(room.min, Coord(room.Size().x, ySplit));
	Boundary room2(Coord(room.min.x, room.min.y + ySplit),
		Coord(room.Size().x, room.Size().y - ySplit));

	// adds the two rooms to the queue that was passed in
	roomsQueue.push(room1);
	roomsQueue.push(room2);
}

bool Algorithms::isOutOfBounds(int dungeonWidth, int dungeonHeight, Coord position)
{
	if (position.x < 0 || position.x >= dungeonWidth ||
		position.y < 0 || position.y >= dungeonHeight)
	{
		return true;
	}

	return false;
}

Coord Algorithms::ChangeDirection(Coord& position, Coord& direction, int dungeonWidth, int dungeonHeight)
{
	return Direction2D::GetRandomTurnDirection(direction);
}