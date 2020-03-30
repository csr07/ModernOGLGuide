
#include <stdio.h>

#include "Geometry.h"
#include "GeometryParser.h"

Geometry::Geometry() :
	filename(""),
	name(NULL),
	vertices(NULL),
	elements(NULL),
	numVertex(0),
	verticesSize(0),
	numTriangles(0),
	elementsSize(0)
{
	printf_s("in Geometry()\n");
}

Geometry::Geometry(const char* f): 
	filename(f),
	name(NULL),
	vertices(NULL),
	elements(NULL),
	numVertex(0),
	verticesSize(0),
	numTriangles(0),
	elementsSize(0)
{
	printf_s("in Geometry(const char*) f: %s\n", f);		
}

Geometry::~Geometry()
{
	printf_s("in ~Geometry()\n");
	if(name)
		delete name;
	if (vertices)
		delete vertices;
	if (elements)
		delete elements;
}