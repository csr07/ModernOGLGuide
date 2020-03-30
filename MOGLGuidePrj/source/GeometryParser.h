#pragma once

class GeometryParser
{
public:
	GeometryParser();	
	~GeometryParser();	

	virtual const char*  CheckForToken(const char* line);
};