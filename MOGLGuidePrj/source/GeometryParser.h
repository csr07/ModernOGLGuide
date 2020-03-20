#pragma once

class Geometry;

class GeometryParser
{
public:
	GeometryParser();
	GeometryParser(Geometry* pg);
	~GeometryParser();	

	virtual void Parse();
	virtual const char*  CheckForToken(const char* line);
	
protected:
	Geometry* pGeometry;
};