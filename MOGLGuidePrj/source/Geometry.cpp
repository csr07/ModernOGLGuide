
#include <stdio.h>

#include "Geometry.h"
#include "GeometryParser.h"

Geometry::Geometry() :
	filename(""),
	name(NULL),
	parser(NULL)
{
	printf_s("in Geometry()\n");
}

Geometry::Geometry(const char* f): 
	filename(f),
	name(NULL),
	parser(NULL)
{
	printf_s("in Geometry(const char*) f: %s\n", f);		
}

Geometry::~Geometry()
{
	if(name)
		delete name;
	if (vertices)
		delete vertices;
	if (elements)
		delete elements;
}