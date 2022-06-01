#pragma once
#include "SquareGrid.h"
#include "Uitls.h"

namespace ProceduralGeneration
{

class MeshGenerator
{
	const int xOffset = 640;
	const int yOffset = 360;

	// The grid to generate the mesh inside
	SquareGrid* m_grid;
	float m_wallHeight;

	vector<Vector3>* m_vertices;
	vector<int>* m_triangles;
	vector<vector<int>>* m_outlines;
	vector<Vector3>* m_wallVertices;
	vector<int>* m_wallTriangles;

	unordered_set<int>* m_checkedVertices;

	std::map<int, vector<Triangle>>* m_triangleMap;

public:
	MeshGenerator();
	~MeshGenerator();

	/// <summary>
	/// Generates a new mesh to go on the grid
	/// </summary>
	/// <param name="map">2D array of numbers for the procedully generated map</param>
	/// <param name="squareSize">The scale of the space taken up by each square on the grid</param>
	void GenerateMesh(Array2D<int>* map, float squareSize, float wallHeight);

	/// <summary>
	/// Produces different triangular shapes from a square based on the value passed in
	/// </summary>
	/// <param name="square">The square object to extract the configuration from</param>
	void TriangulateSquare(Square& square);

	/// <summary>
	/// Creates triangles from a series of points then builds a mesh out of them
	/// </summary>
	/// <param name="points">Takes in a node array of any size contining the points</param>
	void MeshFromPoints(std::vector<Node>* points);

	/// <summary>
	/// Turns points passed in into vertices
	/// </summary>
	/// <param name="points">Takes in a node array of any size contining the points</param>
	void AssignVertices(std::vector<Node>* points);

	// Ceates a triangle out of 3 nodes
	void CreateTriangle(Node a, Node b, Node c);
	// Creates the walls of the caves based on generated layout
	void CreateWallMesh();
	// 
	void AddTriangleToDictionary(int vertexIndexKey, Triangle triangle);
	// 
	void CalculateMeshOutlines();
	// 
	void FollowOutline(int vertexIndex, int outlineIndex);
	// 
	int GetConnectedOutlineVertex(int vertexIndex);
	// 
	bool IsOutlineEdge(int vertexA, int vertexB);

	/*** Getters ***/	
	// Returns all the vertices that make up the mesh
	vector<Vector3>* GetBaseVertices() { return m_vertices; }
	// Returns all the triangles that make up the mesh
	vector<int>* GetBaseTriangles() { return m_triangles; }
	// walls

	vector<Vector3>* GetWallVertices() { return m_wallVertices; }
	vector<int>* GetWallTriangles() { return m_wallTriangles; }
};

}