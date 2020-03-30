
#include <stdio.h>

#include "GeometryParser.h"
#include "Geometry.h"

GeometryParser::GeometryParser()	
{
	printf_s("in GeometryParser()\n");
}

GeometryParser::~GeometryParser()
{
	printf_s("in ~GeometryParser()\n");
}

const char* GeometryParser::CheckForToken(const char* l)
{
	return "hello";
}