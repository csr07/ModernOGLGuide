#include <stdio.h>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h> //GLEW_STATIC and glew.h defined and included before the library we use to create our window

#include <SDL.h>

#include "FileReader.h"
#include "Texture.h"

#include "SimpleGeometry.h"


float verticesTriangle[] = {
	0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

float verticesRectangle[] = {
	//  Position      Color             Texcoords
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
};

GLuint elementsTriangle[] = {
	0, 1, 2
};

GLuint elementsRectangle[] = {
	0, 1, 2,
	2, 3, 0
};

int main(int argc, char* argv[])
{
	printf("Welcome to Modern OpenGL Guide test R2\n");

	/////TEST
			
	

	////END TEST

	SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_Delay(1000);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("Modern OpenGL Guide v2.0 2020", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;			//Glew Experimental and Init after Window and OpenGL Context creation
	glewInit();

	///////////////////////////////////////////////////////////////////////////////

	//GLuint vertexBuffer;				//Testing an OpenGL function loaded by GLEW
	//glGenBuffers(1, &vertexBuffer);
	//printf("Glew Function Test: %u\n", vertexBuffer);

	///////////////////////////////////////////////////////////////////////////////

	Geometry* myRectangle;

	myRectangle = new SimpleGeometry("../Resources/Geometries/Rectangle.geom");	

	int sizet = sizeof(verticesRectangle);
	printf("sizeof(verticesRectangle): %d\n", sizet);

	float* verts = myRectangle->GetVertices();	
	int sizeVertsPtr = sizeof(verts);
	int sizeVertsPtrValue = sizeof(*verts);
	int sizeTotalVertsPtrValue = sizeof(*verts) * myRectangle->GetVerticesLen();
	printf("sizeof(verts): %d\n", sizeVertsPtr);
	printf("sizeof(*verts): %d\n", sizeVertsPtrValue);
	printf("sizeof(*verts) * myRectangle->GetVerticesLen(): %d\n", sizeTotalVertsPtrValue);


	int sizeOriginalElements = sizeof(elementsRectangle);
	int* elements = myRectangle->GetElements();
	int sizeElementsPtr = sizeof(elements);
	int sizeElementsPtrValue = sizeof(*elements);
	int elementsSize = sizeof(*elements) * myRectangle->GetNumTriangles() * 3;

	printf("sizeof(OriginalelementsRectangle): %d\n", sizeOriginalElements);
	printf("sizeof(elements): %d\n", sizeElementsPtr);
	printf("sizeof(*elements): %d\n", sizeElementsPtrValue);
	printf("elementsSize: %d\n", sizeof(*elements) * myRectangle->GetNumTriangles() * 3);

	printf("///////////////////////////////////////////////////////////////////////////////");


	///////////////////////////////////////////////////////////////////////////////

	GLuint vao;							//Creating a VAO before any VBO bound...
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRectangle), verticesRectangle, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*verts) * myRectangle->GetVerticesLen(), verts, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsRectangle), elementsRectangle, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*elements) * myRectangle->GetNumTriangles() * 3, elements, GL_STATIC_DRAW);

	//////////////////////////////////////////////////////////////////////////////
	//Shaders

	char* vSource;
	FileReader::Read("../Resources/Shaders/RectangleShader.vs", &vSource); //relative to ProjectFile, sending as a pointer to pointer	

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glCompileShader(vertexShader);
		
		GLint status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

	delete(vSource);
	vSource = NULL;
	
	char* fSource;
	FileReader::Read("../Resources/Shaders/RectangleShader.fs", fSource); //relative to ProjectFile, sending as a reference to a pointer

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fSource, NULL);
	glCompileShader(fragmentShader);
			
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		char frag_buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, frag_buffer);

	delete(fSource);
	fSource = NULL;
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);    

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//////////////////////////////////////////////////////////////////////////////
	//Attribute pointers

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	//GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	//glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));

	//Activate Texture Units, not needed when only one image used.
	glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

	//////////////////////////////////////////////////////////////////////////////
	//Time stuff
	auto t_start = std::chrono::high_resolution_clock::now();

	//////////////////////////////////////////////////////////////////////////////
	//Texture stuff
	Texture kittenPuppyTex;

	//////////////////////////////////////////////////////////////////////////////
	
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}

		/////////////////////////////////		//Time update
		
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>> (t_now - t_start).count();

		/////////////////////////////////		//Changing Uniforms

		//glUniform3f(uniColor, (sin((double)time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

		/////////////////////////////////		//Drawing the stuff
		
		//glDrawArrays(GL_TRIANGLES, 0, 3);		//Drawing the vertices from vbo directly

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//drawing 6 vertices via element indices..
		
		/////////////////////////////////

		SDL_GL_SwapWindow(window);
	}	

	delete myRectangle;
	myRectangle = NULL;
		
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	printf("bye bye");
	return 0;
}