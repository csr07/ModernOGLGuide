
#include <stdio.h>

#include "SimpleGeometry.h"

SimpleGeometry::SimpleGeometry(const char* fname) : 
	Geometry(fname)	
{	
	//en este punto ya se llam� automaticamente al Constructor por default o se ejecut� la secci�n de inicializaci�n de objetos
	printf_s("in SimpleGeometry(const char*), name: %s\n", fname);
	printf_s("   this->GetFileName() : %s\n",this->GetFileName());


	glGenBuffers(1, &_hvbo);
	glBindBuffer(GL_ARRAY_BUFFER, _hvbo);

	glGenBuffers(1, &_hebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _hebo);

}

SimpleGeometry::~SimpleGeometry()
{
	printf_s("in ~SimpleGeometry()");	
}