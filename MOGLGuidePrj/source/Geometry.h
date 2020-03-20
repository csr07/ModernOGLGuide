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
	const int GetVerticesLen() { return verticesLen; }
	float* GetVertices()const { return vertices; }

	const int GetNumTriangles() { return numTriangles; }
	int* GetElements()const { return elements; }	
	
	void SetName(const char* n)
	{
		int len = strlen(n) + 1;
		name = new char[len];		
		strcpy_s((char*)name, len, n);
	}

	void SetNumVertex(const int n) { numVertex = n; }
	void SetVerticesLen(const int n) { verticesLen = n; }
	void SetVertices(const float* f) { vertices = (float*)f; }		

	void SetNumTriangles(const int n) { numTriangles = n; }
	void SetElements(const int* i) { elements = (int*)i; }

protected:
	const char* filename;

	////geometry info
	const char* name;

	int numVertex;
	int verticesLen;
	float* vertices;	

	int numTriangles; //num Triangles /  Elements = (numTriangles*3)
	int* elements;

	GeometryParser* parser;
};