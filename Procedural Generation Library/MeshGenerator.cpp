#include "MeshGenerator.h"

using namespace ProceduralGeneration;

void ProceduralGeneration::MeshGenerator::GenerateMesh(Array2D<int>* map, float squareSize)
{
    m_grid = new SquareGrid(map, squareSize);
    Array2D<Square>* squares = m_grid->GetSquares();

    //m_vertices = new vector<Vector3>();
    //m_triangles = new vector<int>();

    for (int x = 0; x < squares->getSize(0); x++)
    {
        for (int y = 0; y < squares->getSize(1); y++)
        {
            TriangulateSquare(squares->get(x, y));

        }

    }
}

void ProceduralGeneration::MeshGenerator::TriangulateSquare(Square& square)
{
    Node* points;

    switch (square.configuration)
    {
    case 0: // if no points are selected then we don't have a mesh
        break;

        // 1 point selections
    case 1: // 0001
        points = new Node[3]{ *square.centreBottom, *square.bottomLeft, *square.centreLeft };
        MeshFromPoints(points);
        break;

    case 2: // 0010
        points = new Node[3]{ *square.centreRight, *square.bottomRight, *square.centreBottom };
        MeshFromPoints(points);
        break;

    case 4: // 0100
        points= new Node[3]{ *square.centreTop, *square.topRight, *square.centreRight};
        MeshFromPoints(points);
        break;

    case 8: // 1000
        points = new Node[3]{*square.topLeft, *square.centreTop, *square.centreLeft};
        MeshFromPoints(points);
        break;

        // 2 point selections
    case 3: // 0011
        points = new Node[4]{*square.centreRight, *square.bottomRight, *square.bottomLeft, *square.centreLeft};
        MeshFromPoints(points);
        break;

    case 6: // 0110

        points = new Node[4]{ *square.centreTop, *square.topRight, *square.bottomRight, *square.centreBottom };
        MeshFromPoints(points);
        break;

     case 9: // 1001
         points = new Node[4]{*square.topLeft, *square.centreTop, *square.centreBottom, *square.bottomLeft};
         MeshFromPoints(points);
         break;

     case 12: // 1100
         points = new Node[4]{*square.topLeft, *square.topRight, *square.centreRight, *square.centreLeft};
         MeshFromPoints(points);
         break;

     case 5: // 0101
         points = new Node[6]{*square.centreTop, *square.topRight, *square.centreRight, *square.centreBottom,
             *square.bottomLeft, *square.centreLeft};
         MeshFromPoints(points);
         break;

     case 10: // 1010
         points = new Node[6]{*square.topLeft, *square.centreTop, *square.centreRight, *square.bottomRight,
             *square.centreBottom, *square.centreLeft};
         MeshFromPoints(points);
         break;

         // 3 point selections
     case 7: // 0111
         points = new Node[5]{*square.centreTop, *square.topRight, *square.bottomRight, *square.bottomLeft, *square.centreLeft};
         MeshFromPoints(points);
         break;

     case 11: // 1011
         points = new Node[5]{*square.topLeft, *square.centreTop, *square.centreRight, *square.bottomRight, *square.bottomLeft};
         MeshFromPoints(points);
         break;

     case 13: // 1101
         points = new Node[5]{*square.topLeft, *square.topRight, *square.centreRight, *square.centreBottom, *square.bottomLeft};
         MeshFromPoints(points);
         break;

     case 14: // 1110
         points = new Node[5]{*square.topLeft, *square.topRight, *square.bottomRight, *square.centreBottom, *square.centreLeft};
         MeshFromPoints(points);
         break;

         // 4 point selection:
     case 15: // 1111
         points = new Node[4]{*square.topLeft, *square.topRight, *square.bottomRight, *square.bottomLeft};
         MeshFromPoints(points);
         break;
    }
}

void MeshGenerator::MeshFromPoints(Node points[])
{
    int size = sizeof(points) / sizeof(points[0]);

    AssignVertices(points); // turns points into vertices

    // if there are 3 or more vertices create a triangle      
    if (size >= 3)
        CreateTriangle(points[0], points[1], points[2]);
    // if there are 4 vertices create a second triangles
    if (size >= 4)
        CreateTriangle(points[0], points[2], points[3]);
    // if there are 5 vertices create third triangles
    if (size >= 5)
        CreateTriangle(points[0], points[3], points[4]);
    // if there are 6 vertices create fourth triangles
    if (size >= 6)
        CreateTriangle(points[0], points[3], points[4]);

    delete points;
}

void ProceduralGeneration::MeshGenerator::AssignVertices(Node points[])
{
    int numPoints = sizeof(points) / sizeof(points[0]);

    for (int i = 0; i < numPoints; i++)
    {
        if (points[i].vertexIndex == -1)
        {
            points[i].vertexIndex = (int) m_vertices->size();
            m_vertices->Add(points[i].position);
        }
    }
}

void ProceduralGeneration::MeshGenerator::CreateTriangle(Node a, Node b, Node c)
{
    m_triangles->Add(a.vertexIndex);
    m_triangles->Add(b.vertexIndex);
    m_triangles->Add(c.vertexIndex);
}