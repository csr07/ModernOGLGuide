
#include <stdio.h>

#include "SimpleGeometry.h"
#include "SimpleGeometryParser.h"

SimpleGeometry::SimpleGeometry(const char* fname) : 
	Geometry(fname)	
{		
	//en este punto ya se llam� automaticamente al Constructor por default o se ejecut� la secci�n de inicializaci�n de objetos
	printf_s("in SimpleGeometry(const char*), name: %s\n", fname);
	printf_s("   this->GetFileName() : %s\n",this->GetFileName());
	parser = new SimpleGeometryParser(this);	
	parser->Parse();

	printf_s("Geometry Name: %s\n", this->name);
}

SimpleGeometry::~SimpleGeometry()
{
	printf_s("in ~SimpleGeometry()");
	delete(parser);
	parser = NULL;
}