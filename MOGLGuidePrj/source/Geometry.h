#pragma once
#include <string.h>

class GeometryParser;

class Geometry
{
public:
	Geometry();
	Geometry(const char* f);
	~Geometry();

	const char* GetFileName() const{ return filename; }

	const int GetNumVertex() { return numVertex; }
	const int GetVerticesSize() { return verticesSize; }
	float* GetVertices()const { return vertices; }

	const int GetNumTriangles() { return numTriangles; }
	const int GetElementsSize() { return elementsSize; }
	int* GetElements()const { return elements; }	
	
	void SetName(const char* n)
	{
		int len = strlen(n) + 1;
		name = new char[len];		
		strcpy_s((char*)name, len, n);
	}

	void SetNumVertex(const int n) { numVertex = n; }
	void SetVerticesSize(const int n) { verticesSize = n; }
	void SetVertices(const float* f) { vertices = (float*)f; }		

	void SetNumTriangles(const int n) { numTriangles = n; }
	void SetElementsSize(const int n) { elementsSize = n; }
	void SetElements(const int* i) { elements = (int*)i; }

protected:
	const char* filename;

	////geometry info	

	int numVertex;
	int verticesSize;
	int numTriangles; //num Triangles /  Elements = (numTriangles*3)
	int elementsSize;

	const char* name;
	float* vertices;
	int* elements;		
};