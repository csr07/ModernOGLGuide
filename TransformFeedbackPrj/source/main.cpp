#include <stdio.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>

#include "FileReader.h"

int main(int argc, char* argv[])
{
	printf("Welcome to Modern OpenGL Guide test R2 - Transform Feedback TEST\n");

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("Modern OpenGL Guide v2.0 2020 - Transform Feedback TEST", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	//////////////////////////////////////////////////////////////////////////////////////////////////

	GLuint shader;
	char* source;
	FileReader::Read("../Resources/Shaders/FeedbackShader.vs", source);
		shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		delete(source);

	GLuint gShader;
	char* gSource;
	FileReader::Read("../Resources/Shaders/FeedbackShader.gs", gSource);
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &gSource, nullptr);
		glCompileShader(gShader);
		delete(gSource);

	GLuint program = glCreateProgram();
	glAttachShader(program, shader);
	glAttachShader(program, gShader);

	const GLchar* feedbackVaryings[] = { "outValue" };
	glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(program);
	glUseProgram(program);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	GLint inputAttrib = glGetAttribLocation(program, "inValue");
	glEnableVertexAttribArray(inputAttrib);
	glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Create a Vertex Buffer Object to retrieve the transform feedback values

	GLuint tbo;
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data)*3, nullptr, GL_STATIC_READ); //the output buffer needs to be 3 times the original data sentd

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Query objects

	GLuint query;
	glGenQueries(1, &query);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	glEnable(GL_RASTERIZER_DISCARD);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);
	glBeginTransformFeedback(GL_TRIANGLES);

		glDrawArrays(GL_POINTS, 0, 5);

	glEndTransformFeedback();
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	glFlush();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//Retrieve query data
	GLuint primitives;
	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitives);

	//Retrieve feedback Data
	GLfloat feedback[15];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

	printf_s("Primitives Written: %d \n", primitives);

	printf_s("Transform Feedback results: \n");
	for (int i = 0; i < 15; i++)
	{
		printf_s("%f \n", feedback[i]);
	}	

	//////////////////////////////////////////////////////////////////////////////////////////////////

	glDeleteShader(shader);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &tbo);

	//////////////////////////////////////////////////////////////////////////////////////////////////

	SDL_GL_DeleteContext(context);
	SDL_Quit();
	printf("bye bye \n");

	return 0;
}