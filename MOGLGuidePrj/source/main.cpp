#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h> //GLEW_STATIC and glew.h defined and included before the library we use to create our window

#include <SDL.h>


int main(int argc, char* argv[])
{
	printf("Welcome to Modern OpenGL Guide test R2\n");

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

	GLuint vertexBuffer;				//Testing an OpenGL function loaded by GLEW
	glGenBuffers(1, &vertexBuffer);
	printf("Glew Function Test: %u\n", vertexBuffer);
	
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}

		SDL_GL_SwapWindow(window);
	}	
		
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	printf("bye bye");
	return 0;
}