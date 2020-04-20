#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>

#include "FileReader.h"

void CreateShadersAndProgram(const char* vsPath, const char* gsPath, const char* fsPath, GLuint& vertexShader,GLuint& geometryShader, GLuint& fragmentShader, GLuint& shaderProgram)
{
	char* vSource;
	FileReader::Read(vsPath, &vSource); //relative to ProjectFile, sending as a pointer to pointer	

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

	delete(vSource);
	vSource = NULL;

	//////////////////////////////////////////////////////////////////////////////////////////////////

	char* fSource;
	FileReader::Read(fsPath, fSource); //relative to ProjectFile, sending as a reference to a pointer

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	char frag_buffer[512];
	glGetShaderInfoLog(fragmentShader, 512, NULL, frag_buffer);

	delete(fSource);
	fSource = NULL;

	//////////////////////////////////////////////////////////////////////////////////////////////////

	char* gSource;
	FileReader::Read(gsPath, gSource);

	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometryShader, 1, &gSource, NULL);
	glCompileShader(geometryShader);

	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &status);
	char geom_buffer[512];
	glGetShaderInfoLog(geometryShader, 512, NULL, geom_buffer);

	delete(gSource);
	gSource = NULL;

	//////////////////////////////////////////////////////////////////////////////////////////////////

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
}


int main(int argc, char* argv[])
{
	printf("Welcome to Modern OpenGL Guide test R2 - Geometry Shaders TEST\n");

	SDL_Init(SDL_INIT_EVERYTHING);	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("Modern OpenGL Guide v2.0 2020 - Geometry Shaders TEST", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	
	GLuint vShader, fShader, gShader, program;	
	CreateShadersAndProgram("../Resources/Shaders/GeometryShader.vs", 
							"../Resources/Shaders/GeometryShader.gs",
							"../Resources/Shaders/GeometryShader.fs",							
							vShader, 
							gShader, 
							fShader, 
							program);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	GLuint vbo;
	glGenBuffers(1, &vbo);

	float points[] = {
		-0.45f, 0.45f, 1.0f, 0.0f, 0.0f, 4.0f,
		 0.45f, 0.45f, 0.0f, 1.0f, 0.0f, 8.0f,
		 0.45f, -0.45f, 0.0f, 0.0f, 1.0f, 16.0f,
		-0.45f, -0.45f, 1.0f, 1.0f, 0.0f, 32.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLint posAttrib = glGetAttribLocation(program, "pos");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);

	GLint colorAttrib = glGetAttribLocation(program, "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2*sizeof(float)));

	GLint sidesAttrib = glGetAttribLocation(program, "sides");
	glEnableVertexAttribArray(sidesAttrib);
	glVertexAttribPointer(sidesAttrib, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

	//////////////////////////////////////////////////////////////////////////////////////////////////

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;			
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_POINTS, 0, 4);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();
	printf("bye bye");
	return 0;
}