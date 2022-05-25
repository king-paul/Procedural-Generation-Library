#pragma once
#include "SquareGrid.h"

namespace ProceduralGeneration
{

class MeshGenerator
{
	const int xOffset = 640;
	const int yOffset = 360;

	// The grid to generate the mesh inside
	SquareGrid* m_grid;

	vector<Vector3>* m_vertices;
	vector<int>* m_triangles;

public:

	MeshGenerator() { 
		m_vertices = new vector<Vector3>();
		m_triangles = new vector<int>();
	}
	~MeshGenerator() 
	{
		delete m_vertices;

		

		delete m_triangles;
		delete m_grid;
	}

	/// <summary>
	/// Generates a new mesh to go on the grid
	/// </summary>
	/// <param name="map">2D array of numbers for the procedully generated map</param>
	/// <param name="squareSize">The scale of the space taken up by each square on the grid</param>
	void GenerateMesh(Array2D<int>* map, float squareSize);

	/// <summary>
	/// Produces different triangular shapes from a square based on the value passed in
	/// </summary>
	/// <param name="square">The square object to extract the configuration from</param>
	void TriangulateSquare(Square& square);

	/// <summary>
	/// Creates triangles from a series of points then builds a mesh out of them
	/// </summary>
	/// <param name="points">Takes in a node array of any size contining the points</param>
	void MeshFromPoints(Node points[]);

	/// <summary>
	/// Turns points passed in into vertices
	/// </summary>
	/// <param name="points">Takes in a node array of any size contining the points</param>
	void AssignVertices(Node points[]);

	// cCeates a triangle out of 3 nodes
	void CreateTriangle(Node a, Node b, Node c);

	/*** Getters ***/	
	// Returns all the vertices that make up the mesh
	vector<Vector3>* GetVertices() { return m_vertices; }
	// Returns all the triangles that make up the mesh
	vector<int>* triangle() { return m_triangles; }

};

}