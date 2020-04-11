#include <stdio.h>
#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h> //GLEW_STATIC and glew.h defined and included before the library we use to create our window

#include <SDL.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "FileReader.h"
#include "Texture.h"

#include "SimpleGeometry.h"
#include "SimpleGeometryParser.h"

int main(int argc, char* argv[])
{
	printf("Welcome to Modern OpenGL Guide test R2\n");

	SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_Delay(1000);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_Window* window = SDL_CreateWindow("Modern OpenGL Guide v2.0 2020", 100, 100, 1024, 768, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;			//Glew Experimental and Init after Window and OpenGL Context creation
	glewInit();

	///////////////////////////////////////////////////////////////////////////////
	//Creating a VAO before any VBO bound...
	//This vao will be enough for these program because all geometries will have the same vertex format
	GLuint vao;							
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	///////////////////////////////////////////////////////////////////////////////
	//Create a Geometry
	
	SimpleGeometryParser* parser = new SimpleGeometryParser();	
	SimpleGeometry* myRectangle = parser->GetSimpleGeometry("../Resources/Geometries/Cube.geom");
	
		
	glBindBuffer(GL_ARRAY_BUFFER, myRectangle->GetHandlerVertexBuffer());
	glBufferData(GL_ARRAY_BUFFER, myRectangle->GetVerticesSize(), myRectangle->GetVertices(), GL_STATIC_DRAW);

	if (myRectangle->GetNumTriangles() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myRectangle->GetHandlerElementBuffer());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, myRectangle->GetElementsSize(), myRectangle->GetElements(), GL_STATIC_DRAW);
	}

	//////////////////////////////////////////////////////////////////////////////
	//Shaders

	char* vSource;
	FileReader::Read("../Resources/Shaders/CubeShader.vs", &vSource); //relative to ProjectFile, sending as a pointer to pointer	

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
	FileReader::Read("../Resources/Shaders/CubeShader.fs", fSource); //relative to ProjectFile, sending as a reference to a pointer

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
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint uniTime = glGetUniformLocation(shaderProgram, "time");	
	GLint uniTimeSin = glGetUniformLocation(shaderProgram, "timeSin");	
	//glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));

	//Activate Texture Units, not needed when only one image used.
	//2 steps:
	GLuint texLocation = glGetUniformLocation(shaderProgram, "texKitten");
	glUniform1i(texLocation, 0); //sending the number of texture unit
	//in 1 step
	glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");

	GLint uniOverrideColor = glGetUniformLocation(shaderProgram, "overrideColor");

	//////////////////////////////////////////////////////////////////////////////
	//Time stuff
	auto t_start = std::chrono::high_resolution_clock::now();

	//////////////////////////////////////////////////////////////////////////////
	//Texture stuff
	Texture kittenPuppyTex;

	//////////////////////////////////////////////////////////////////////////////
	//Transformation stuff
	/*GLfloat speed = 0.0f;
	GLfloat angle = -45.0f;*/

	//////////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);


	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;			
			if (windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
			/*if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_SPACE)
			{
				speed = 180.0f;
			}	*/		
		}		

		/////////////////////////////////		//Clear Buffers
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		/////////////////////////////////		//Time update
		
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>> (t_now - t_start).count();

		/////////////////////////////////		//Changing Uniforms

		glUniform1f(uniTimeSin, (sin((double)time * 1.0f) + 1.0f) / 2.0f);
		glUniform1f(uniTime, time);

		/////////////////////////////////		//Transformation Uniforms
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(time) * 15, glm::vec3(0.0f, 0.0f, 1.0f));

		GLfloat s = sin(time * 2.0f) * 0.25f + 1.5f;
		model = glm::scale(model, glm::vec3(s, s, s)); //scaling the image with a sin function

		glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, -5.0f, 2.0f), //camera position
			glm::vec3(0.0f, 0.0f, -0.5f), //point of interest
			glm::vec3(0.0f, 0.0f, 1.0f)  //y is UP,    x,z is the "plane"  ground			
			);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 1.0f, 10.0f);

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		/*speed /= 1.0f + time;
		angle += 5 * speed * time;*/

		/////////////////////////////////		//Drawing the stuff
		
		if (myRectangle->GetNumTriangles() == 0)
			glDrawArrays(GL_TRIANGLES, 0, 36);		//Drawing the vertices from vbo directly
		else
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//6 elements or 6 vertexElements ~ 2 triangles

		//Draw the floor
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);	// Pass always writting 1s
		glStencilOp(GL_KEEP,				//if stencil test fails
					GL_KEEP,				//stencil passes, depth fails
					GL_REPLACE);			//stencil passes, depth passes
		glStencilMask(0xFF);				//Write to Stencil Buffer

		glDepthMask(GL_FALSE);				//Don't write to Depth Buffer
		glClear(GL_STENCIL_BUFFER_BIT);		//Clear Stencil Buffer with 0s

		glDrawArrays(GL_TRIANGLES, 36, 6);		

		//Transform and draw the second cube
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 0xFF);	//PassTest if stencil value is 1
		glStencilMask(0x00);

		model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)), glm::vec3(1, 1, -1));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(uniOverrideColor, 0.4f, 0.4f, 0.4f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(uniOverrideColor, 1.0f, 1.0f, 1.0f);
		
		glDisable(GL_STENCIL_TEST);
		/////////////////////////////////

		SDL_GL_SwapWindow(window);
	}	

	delete myRectangle;
	myRectangle = NULL;

	delete parser;
	parser = NULL;
		
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	printf("bye bye");
	return 0;
}