
#include <stdio.h>

#include "GeometryParser.h"
#include "Geometry.h"

GeometryParser::GeometryParser() :
	pGeometry(NULL)
{
	printf_s("in GeometryParser()\n");
}

GeometryParser::GeometryParser(Geometry* pg) : 
	pGeometry(pg)
{
	printf_s("in GeometryParser(Geometry*)\n");
}

GeometryParser::~GeometryParser()
{
	printf_s("in ~GeometryParser()\n");
}

////////////////////////////

void GeometryParser::Parse()
{
	printf_s("in GeometryParser::Parse()\n");	
	
}

const char* GeometryParser::CheckForToken(const char* l)
{
	return "hello";
}