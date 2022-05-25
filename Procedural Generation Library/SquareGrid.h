#pragma once

#include "Types.h"

namespace ProceduralGeneration
{
    /// <summary>
    /// Storse a 2D array of squares
    /// </summary>
    class SquareGrid
    {
        Array2D<Square>* squares;
        
    public:
        /// <summary>
        /// Fills the 2D array of squares using parameters
        /// </summary>
        /// <param name="map">2D array of integers to be passed in from the map generator</param>
        /// <param name="squareSize">The scale of the squares inside the grid</param>
        SquareGrid(Array2D<int>* map, float squareSize)
        {
            int nodeCountX = map->getSize(0); // Total number of colums in the grid
            int nodeCountY = map->getSize(1); // Total number of rows in the grid

            // width and height based on the number of rows and columns and the size of each square
            float mapWidth = nodeCountX * squareSize;
            float mapHeight = nodeCountY * squareSize;

            // 2d array of controls nodes to be placed on the square
            Array2D<ControlNode> controlNodes(nodeCountX, nodeCountY);

            // iterates through the grid
            for (int x = 0; x < nodeCountX; x++)
            {
                for (int y = 0; y < nodeCountY; y++)
                {
                    // gets the position of next control node  
                    Vector3 pos = Vector3(-mapWidth / 2 + x * squareSize + squareSize / 2, 0,
                        -mapHeight / 2 + y * squareSize + squareSize / 2);

                    // add a new control node to the array based the position, whether the value in the array is a wall
                    // and the size of the square
                    controlNodes.set(x, y, ControlNode(pos, map->get(x, y) == 1, squareSize));
                }
            }

            // defines the sise of the square array and fills it with square objects
            squares = new Array2D<Square>(nodeCountX - 1, nodeCountY - 1);
            // iterates through the squares array
            for (int x = 0; x < nodeCountX - 1; x++)
            {
                for (int y = 0; y < nodeCountY - 1; y++)
                {
                    // creates a new square and defines the 4 control nodes on it
                    squares->set(x, y, Square(&controlNodes.get(x, y + 1), &controlNodes.get(x + 1, y + 1), 
                                 &controlNodes.get(x + 1, y), &controlNodes.get(x, y)));
                }
            }
        }

        Array2D<Square>* GetSquares() { return squares; }

        ~SquareGrid()
        {
            delete squares;
        }
    };

}