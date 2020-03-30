#pragma once

#include "Geometry.h"
#include "GL/glew.h"


//SimpleGeometry equals to a Geometry ready for rendering with OpenGL, will include handlers for its VertexBufferObject and ElementsBufferObject

class SimpleGeometry : public Geometry
{
public:	
	SimpleGeometry(const char* f);
	~SimpleGeometry();

	GLuint GetHandlerVertexBuffer() const { return _hvbo; };
	GLuint GetHandlerElementBuffer() const { return _hebo; };

private: 
	GLuint _hvbo;
	GLuint _hebo;
};