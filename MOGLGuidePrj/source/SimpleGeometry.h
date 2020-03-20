#pragma once

#include "Geometry.h"

class SimpleGeometry : public Geometry
{
public:	
	SimpleGeometry(const char* f);
	~SimpleGeometry();

	//virtual const char* GetFileName() const { return filename; }
};