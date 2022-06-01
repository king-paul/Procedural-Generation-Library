#include "MeshGenerator.h"

using namespace ProceduralGeneration;

MeshGenerator::MeshGenerator()
{
    m_vertices = new vector<Vector3>();
    m_triangles = new vector<int>();
    m_outlines = new vector<vector<int>>();
    m_wallVertices = new vector<Vector3>();
    m_wallTriangles = new vector<int>();

    m_checkedVertices = new unordered_set<int>();
    m_triangleMap = new map<int, vector<Triangle>>();

    m_grid = nullptr;
}

MeshGenerator::~MeshGenerator()
{
    // clear vectors
    m_vertices->clear();
    m_triangles->clear();

    for (vector<int>& outline : *m_outlines)
        outline.clear();

    m_outlines->clear();
    m_wallVertices->clear();
    m_wallTriangles->clear();
    m_checkedVertices->clear();
    m_triangleMap->clear();

    // delete memory from heap
    delete m_vertices;
    delete m_triangles;
    delete m_outlines;
    delete m_grid;
    delete m_wallVertices;
    delete m_wallTriangles;
    delete m_checkedVertices;
    delete m_triangleMap;
}

void ProceduralGeneration::MeshGenerator::GenerateMesh(Array2D<int>* map, float squareSize, float wallHeight)
{
    m_grid = new SquareGrid(map, squareSize);
    Array2D<Square>* squares = m_grid->GetSquares();
    m_wallHeight = wallHeight;

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
    std::vector<Node> points;

    switch (square.configuration)
    {
        case 0: // if no points are selected then we don't have a mesh
            break;

        // 1 point selections
        case 1: // 0001

            //points.push_back( (square.centreBottom);
            points = { square.centreBottom, square.bottomLeft, square.centreLeft};
            MeshFromPoints(&points);
            break;

        case 2: // 0010
            points = { square.centreRight, square.bottomRight, square.centreBottom };
            MeshFromPoints(&points);
            break;

        case 4: // 0100
            points= {square.centreTop, square.topRight, square.centreRight};
            MeshFromPoints(&points);
            break;

        case 8: // 1000
            points = {square.topLeft, square.centreTop, square.centreLeft};
            MeshFromPoints(&points);
            break;

        // 2 point selections
        case 3: // 0011
            points = {square.centreRight, square.bottomRight, square.bottomLeft, square.centreLeft};
            MeshFromPoints(&points);
            break;

        case 6: // 0110
            points = {square.centreTop, square.topRight, square.bottomRight, square.centreBottom };
            MeshFromPoints(&points);
            break;

         case 9: // 1001
             points = {square.topLeft, square.centreTop, square.centreBottom, square.bottomLeft};
             MeshFromPoints(&points);
             break;

         case 12: // 1100
             points = {square.topLeft, square.topRight, square.centreRight, square.centreLeft};
             MeshFromPoints(&points);
             break;

         case 5: // 0101
             points = {square.centreTop, square.topRight, square.centreRight, square.centreBottom,
                 square.bottomLeft, square.centreLeft};
             MeshFromPoints(&points);
             break;

         case 10: // 1010
             points ={square.topLeft, square.centreTop, square.centreRight, square.bottomRight,
                 square.centreBottom, square.centreLeft};
             MeshFromPoints(&points);
             break;

         // 3 point selections
         case 7: // 0111
             points = {square.centreTop, square.topRight, square.bottomRight, square.bottomLeft, square.centreLeft};
             MeshFromPoints(&points);
             break;

         case 11: // 1011
             points = {square.topLeft, square.centreTop, square.centreRight, square.bottomRight, square.bottomLeft};
             MeshFromPoints(&points);
             break;

         case 13: // 1101
             points = {square.topLeft, square.topRight, square.centreRight, square.centreBottom, square.bottomLeft};
             MeshFromPoints(&points);
             break;

         case 14: // 1110
             points = {square.topLeft, square.topRight, square.bottomRight, square.centreBottom, square.centreLeft};
             MeshFromPoints(&points);
             break;

         // 4 point selection:
         case 15: // 1111
             points = {square.topLeft, square.topRight, square.bottomRight, square.bottomLeft};
             MeshFromPoints(&points);
             break;
    }
}

void MeshGenerator::MeshFromPoints(std::vector<Node>* points)
{
    int size = points->size();// (int)sizeof(points) / (int)sizeof(points[0]);

    AssignVertices(points); // turns points into vertices

    // if there are 3 or more vertices create a triangle
    if (size >= 3)
        CreateTriangle(((*points))[0], (*points)[1], ( *points)[2]);
    // if there are 4 vertices create a second triangles
    if (size >= 4)
        CreateTriangle((*points)[0], (*points)[2], (*points)[3]);
    // if there are 5 vertices create third triangles
    if (size >= 5)
        CreateTriangle((*points)[0], (*points)[3], (*points)[4]);
    // if there are 6 vertices create fourth triangles
    if (size >= 6)
        CreateTriangle((*points)[0], (*points)[3], (*points)[4]);    
}

void ProceduralGeneration::MeshGenerator::AssignVertices(std::vector<Node>* points)
{
    //int numPoints = (int) sizeof(points) / (int) sizeof(points[0]);

    for (int i = 0; i < points->size(); i++)
    {
        if ((*points)[i].vertexIndex == -1)
        {
            (*points)[i].vertexIndex = (int)m_vertices->size();
                m_vertices->Add((*points)[i].position);
        }
    }
}

void ProceduralGeneration::MeshGenerator::CreateTriangle(Node a, Node b, Node c)
{
    m_triangles->Add(a.vertexIndex);
    m_triangles->Add(b.vertexIndex);
    m_triangles->Add(c.vertexIndex);
}

void ProceduralGeneration::MeshGenerator::CreateWallMesh()
{
    CalculateMeshOutlines();

    // iterate through all the outlines adding each of the vertices
    // to the wall vertices
    for(vector<int> outline : *m_outlines) {
        for (int i = 0; i < outline.size() - 1; i++)
        {
            int startIndex = (int) m_wallVertices->size();

            m_wallVertices->Add((*m_vertices)[outline[i]]); // left vertex
            m_wallVertices->Add((*m_vertices)[outline[i + 1]]); // right vertex
            m_wallVertices->Add((*m_vertices)[outline[i]] - Vector3::Up() * m_wallHeight); // bottom left vertex
            m_wallVertices->Add((*m_vertices)[outline[i + 1]] - Vector3::Up() * m_wallHeight); // bottom right vertex

            // Add indices to wall trianges by winding anti-clockwise around walls

            // first triangle in square
            m_wallTriangles->Add(startIndex + 0);
            m_wallTriangles->Add(startIndex + 2);
            m_wallTriangles->Add(startIndex + 3);
            // second triangle in square
            m_wallTriangles->Add(startIndex + 3);
            m_wallTriangles->Add(startIndex + 1);
            m_wallTriangles->Add(startIndex + 0);

        }
    }
}

void ProceduralGeneration::MeshGenerator::AddTriangleToDictionary(int vertexIndexKey, Triangle triangle)
{
    // checks if the triangle already contains the key        
    if (m_triangleMap->find(vertexIndexKey) != m_triangleMap->end())
    {
        // if it does then add the triangle to the list
        m_triangleMap->at(vertexIndexKey).push_back(triangle);
    }
    else
    {
        // otherwise create a new list of triangles and add
        // the triangle to the list
        vector<Triangle> triangleList;
        triangleList.Add(triangle);

        std::pair<int, vector<Triangle>> pair(vertexIndexKey, triangleList);

        m_triangleMap->insert(pair);
    }
}

void ProceduralGeneration::MeshGenerator::CalculateMeshOutlines()
{
    for (int vertexIndex = 0; vertexIndex < m_vertices->size(); vertexIndex++)
    {
        
        if (!IsInSet(m_checkedVertices, vertexIndex))
        {
            int newOutLineVertex = GetConnectedOutlineVertex(vertexIndex);

            if (newOutLineVertex != -1)
            {
                m_checkedVertices->insert(vertexIndex);

                vector<int> newOutline;
                newOutline.Add(vertexIndex);
                m_outlines->Add(newOutline);

                FollowOutline(newOutLineVertex, (int) m_outlines->size() - 1);
                (&(*m_outlines)[m_outlines->size() - 1])->Add(vertexIndex);
            }
        }
    }

}

void ProceduralGeneration::MeshGenerator::FollowOutline(int vertexIndex, int outlineIndex)
{
    (&(*m_outlines)[outlineIndex])->Add(vertexIndex);
    m_checkedVertices->insert(vertexIndex);

    int nextVertexIndex = GetConnectedOutlineVertex(vertexIndex);

    if (nextVertexIndex != -1) {
        FollowOutline(nextVertexIndex, outlineIndex);
    }
}

int ProceduralGeneration::MeshGenerator::GetConnectedOutlineVertex(int vertexIndex)
{
    vector<Triangle> trianglesContainingVertex = m_triangleMap->at(vertexIndex);

    // look through the vertices of each triangle connected to the vertex        
    for (int i = 0; i < trianglesContainingVertex.size(); i++) {
        Triangle triangle = trianglesContainingVertex[i];

        for (int j = 0; j < 3; j++) {
            int vertexB = triangle.get(j);

            if (vertexB != vertexIndex && !IsInSet(m_checkedVertices, vertexB))
            {
                // if it has an outline edge connected to it return it
                if (IsOutlineEdge(vertexIndex, vertexB))
                {
                    return vertexB;
                }
            }

        }
    }

    return -1; // no outline edge was found
}

bool ProceduralGeneration::MeshGenerator::IsOutlineEdge(int vertexA, int vertexB)
{
    // Get a list of all triangles that vertex A belongs to
    vector<Triangle>* trianglesContainingVertexA = &m_triangleMap->at(vertexA);

    // Check how many of those triangles belong to vertex B
    int sharedTriangleCount = 0;
    for (int i = 0; i < trianglesContainingVertexA->size(); i++)
    {
        if ((*trianglesContainingVertexA)[i].Contains(vertexB))
        {
            sharedTriangleCount++;
            if (sharedTriangleCount > 1)
            {
                break;
            }
        }
    }

    // If vertex A and vertex B only share one common triangle then
    // it is an outline edge
    return sharedTriangleCount == 1;
}
