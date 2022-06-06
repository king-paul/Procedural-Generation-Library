#include "SquareGrid.h"

using namespace ProceduralGeneration;

SquareGrid::SquareGrid(Array2D<int>* map, float squareSize)
{
    m_squareCountX = map->getSize(1); // Total number of colums in the grid
    m_squareCountY = map->getSize(0); // Total number of rows in the grid

    // width and height based on the number of rows and columns and the size of each square
    float mapWidth = m_squareCountX * squareSize;
    float mapHeight = m_squareCountY * squareSize;

    // 2d array of controls nodes to be placed on the square
    Array2D<ControlNode> controlNodes(m_squareCountX, m_squareCountY);

    // iterates through the grid
    for (int x = 0; x < m_squareCountX; x++)
    {
        for (int y = 0; y < m_squareCountY; y++)
        {
            // gets the position of next control node  
            Vector2 pos = Vector2(-mapWidth  / 2 + x * squareSize + squareSize / 2,
                                  -mapHeight / 2 + y * squareSize + squareSize / 2);

            // add a new control node to the array based the position, whether the value in the array is a wall
            // and the size of the square
            controlNodes.set(x, y, ControlNode(pos, map->get(x, y) == 1, squareSize));
        }
    }

    // defines the sise of the square array and fills it with square objects
    squares = new Array2D<Square>(m_squareCountX - 1, m_squareCountY - 1);
    // iterates through the squares array
    for (int x = 0; x < m_squareCountX - 1; x++)
    {
        for (int y = 0; y < m_squareCountY - 1; y++)
        {
            // creates a new square and defines the 4 control nodes on it
            Square nextSquare(controlNodes.at(x, y + 1), // top left
                              controlNodes.at(x + 1, y + 1), // top right
                              controlNodes.at(x + 1, y), // bottom right
                              controlNodes.at(x, y)); // bootom left

            //system("cls");
            //cout << "Square x:" << x << ", y:" << y << endl;
            //nextSquare.Draw();

            // connnect nodes based on configuration
            squares->set(x, y, nextSquare);
        }
    }
}

void SquareGrid::TriangulateSquare(Square& square)
{
    switch (square.configuration)
    {
    case 0: // if no points are selected then we don't have a mesh
        break;

        // 1 point selections
    case 1: // 0001
        square.nodesUsed = { &square.centreBottom, &square.bottomLeft, &square.centreLeft };
        break;

    case 2: // 0010
        square.nodesUsed ={ &square.centreRight, &square.bottomRight, &square.centreBottom };
        
        break;

    case 4: // 0100
        square.nodesUsed ={ &square.centreTop, &square.topRight, &square.centreRight };
        
        break;

    case 8: // 1000
        square.nodesUsed ={ &square.topLeft, &square.centreTop, &square.centreLeft };
        
        break;

        // 2 point selections
    case 3: // 0011
        square.nodesUsed ={ &square.centreRight, &square.bottomRight, &square.bottomLeft, &square.centreLeft };
        
        break;

    case 6: // 0110
        square.nodesUsed ={ &square.centreTop, &square.topRight, &square.bottomRight, &square.centreBottom };
        
        break;

    case 9: // 1001
        square.nodesUsed ={ &square.topLeft, &square.centreTop, &square.centreBottom, &square.bottomLeft };
        
        break;

    case 12: // 1100
        square.nodesUsed ={ &square.topLeft, &square.topRight, &square.centreRight, &square.centreLeft };
        
        break;

    case 5: // 0101
        square.nodesUsed ={ &square.centreTop, &square.topRight, &square.centreRight, &square.centreBottom,
            &square.bottomLeft, &square.centreLeft };
        
        break;

    case 10: // 1010
        square.nodesUsed ={ &square.topLeft, &square.centreTop, &square.centreRight, &square.bottomRight,
            &square.centreBottom, &square.centreLeft };
        
        break;

        // 3 point selections
    case 7: // 0111
        square.nodesUsed ={ &square.centreTop, &square.topRight, &square.bottomRight, &square.bottomLeft, &square.centreLeft };
        
        break;

    case 11: // 1011
        square.nodesUsed ={ &square.topLeft, &square.centreTop, &square.centreRight, &square.bottomRight, &square.bottomLeft };
        
        break;

    case 13: // 1101
        square.nodesUsed ={ &square.topLeft, &square.topRight, &square.centreRight, &square.centreBottom, &square.bottomLeft };
        
        break;

    case 14: // 1110
        square.nodesUsed ={ &square.topLeft, &square.topRight, &square.bottomRight, &square.centreBottom, &square.centreLeft };
        
        break;

        // 4 point selection:
    case 15: // 1111
        square.nodesUsed ={ &square.topLeft, &square.topRight, &square.bottomRight, &square.bottomLeft };
        
        break;
    }
}

void SquareGrid::PrintConfigurations()
{
    for (int x = 0; x < m_squareCountX; x++)
    {
        for (int y = 0; y < m_squareCountY; y++)
        {
            cout << squares->get(x, y).configuration << ',';
        }

        cout << endl;
    }

}
