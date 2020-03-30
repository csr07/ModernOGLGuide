#pragma once

#include "GeometryParser.h"

class SimpleGeometry;

class SimpleGeometryParser : public GeometryParser
{
public:
	static enum enumTokens { NAME, VERTICES, TRIS, NONE };

	SimpleGeometryParser();	
	~SimpleGeometryParser();	

	
	SimpleGeometry* GetSimpleGeometry(const char* filename);
	virtual const char* CheckForToken(const char* line);
	const enumTokens CheckForTokenEnum(const char* line);	

	const char* simpleGeometryTokens[5] = { "#Name", "#Vertices", "#Triangles" };

	
};