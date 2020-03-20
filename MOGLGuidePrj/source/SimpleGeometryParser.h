#pragma once

#include "GeometryParser.h"


class SimpleGeometryParser : public GeometryParser
{
public:
	static enum enumTokens { NAME, VERTICES, TRIS, NONE };

	SimpleGeometryParser() = default;
	SimpleGeometryParser(Geometry* pg);
	~SimpleGeometryParser();	

	virtual void Parse();
	virtual const char* CheckForToken(const char* line);
	const enumTokens CheckForTokenEnum(const char* line);	

	const char* simpleGeometryTokens[5] = { "#Name", "#Vertices", "#Triangles" };

	
};