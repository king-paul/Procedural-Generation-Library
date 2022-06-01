#pragma once

#include "Types.h"

namespace ProceduralGeneration
{
	struct Node
	{
		Vector2 position;
		int vertexIndex;

		Node() {
			position = Vector2::Zero();
			vertexIndex = -1;
		}

		Node(Vector2 pos)
		{
			position = pos;
			vertexIndex = -1;
		}
	};

	struct ControlNode : Node
	{
		bool active;
		Node above, right; // The positions above and to the right of the control node

		ControlNode()
		{
			active = false;
			//above = nullptr;
			//right = nullptr;
		}

		ControlNode(Vector2 pos, bool active, float squareSize) : Node(pos), active(active)
		{
			Vector2 forwardVector = Vector2::Up() * ((float)squareSize / 2.0f);
			Vector2 rightVector = Vector2::Right() * ((float)squareSize / 2.0f);

			// set the position to the distance above
			above = Node(position + forwardVector);
			// set the position to the distance to the right
			right = Node(position + rightVector);
		}

		~ControlNode()
		{
			//delete above;
			//delete right;
		}

	};

	struct Square
	{
		ControlNode topLeft, topRight, bottomRight, bottomLeft;
		Node centreTop, centreRight, centreBottom, centreLeft;

		short configuration; // value is between 0000 and 1111
		vector<Node*> nodesUsed;
		
		Square()
		{			
			//topLeft = nullptr;
			//topRight = nullptr;
			//bottomLeft = nullptr;
			//bottomRight = nullptr;

			//centreTop = nullptr;
			//centreRight = nullptr;
			//centreBottom = nullptr;
			//centreLeft = nullptr;

			configuration = 0000;
		}

		Square(ControlNode topLeft, ControlNode topRight, ControlNode bottomRight, ControlNode bottomLeft)
		{
			// set the cornor nodes
			this->topLeft = topLeft;
			this->topRight = topRight;
			this->bottomRight = bottomRight;
			this->bottomLeft = bottomLeft;

			// set center nodes to the nodes connected to the corner nodes
			centreTop = topLeft.right;
			centreRight = bottomRight.above;
			centreBottom = bottomLeft.right;
			centreLeft = bottomLeft.above;

			configuration = 0;

			// turns on the appropriate bit in the four bit number base on
			// which control nodes are active
			if (topLeft.active)
				configuration += 8; // turns on the first bit
			if (topRight.active)
				configuration += 4; // turns on the second bit
			if (bottomRight.active)
				configuration += 2; // turns on the thrid bit
			if (bottomLeft.active)
				configuration += 1; // turns on the fourth bit			
		}
	};

    /// <summary>
    /// Storse a 2D array of squares
    /// </summary>
    class SquareGrid
    {
        Array2D<Square>* squares;
		int m_nodeCountX, m_nodeCountY;
        
    public:
        /// <summary>
        /// Fills the 2D array of squares using parameters
        /// </summary>
        /// <param name="map">2D array of integers to be passed in from the map generator</param>
        /// <param name="squareSize">The scale of the squares inside the grid</param>
		SquareGrid(Array2D<int>* map, float squareSize);
		void TriangulateSquare(Square& square);
		void PrintConfigurations();		

        Array2D<Square>* GetSquares() { return squares; }

        ~SquareGrid()
        {
            delete squares;
        }
    };

}