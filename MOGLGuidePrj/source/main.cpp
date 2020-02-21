#include <stdio.h>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h> //GLEW_STATIC and glew.h defined and included before the library we use to create our window

#include <SDL.h>

float vertices[] = {
	0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

const char* vertexSource = R"glsl(
	#version 150 core

	in vec2 position;
	in vec3 color;

	out vec3 Color;

	void main()
	{
		Color = color;
		gl_Position = vec4(position, 0.0, 1.0);
	}
)glsl";

const char* fragmentSource = R"glsl(
	#version 150 core
	
	//uniform vec3 triangleColor;
	in vec3 Color;

	out vec4 outColor;

	void main()
	{
		outColor = vec4(Color, 1.0);
	}
)glsl";


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

	///////////////////////////////////////////////////////////////////////////////

	//GLuint vertexBuffer;				//Testing an OpenGL function loaded by GLEW
	//glGenBuffers(1, &vertexBuffer);
	//printf("Glew Function Test: %u\n", vertexBuffer);

	GLuint vao;							//Creating a VAO before any VBO bound...
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//////////////////////////////////////////////////////////////////////////////

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
		
		GLint status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
			
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		char frag_buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, frag_buffer);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);    

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//////////////////////////////////////////////////////////////////////////////

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	//GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	//glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	//////////////////////////////////////////////////////////////////////////////
	//Time stuff
	auto t_start = std::chrono::high_resolution_clock::now();

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
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		/////////////////////////////////

		SDL_GL_SwapWindow(window);
	}	
		
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	printf("bye bye");
	return 0;
}