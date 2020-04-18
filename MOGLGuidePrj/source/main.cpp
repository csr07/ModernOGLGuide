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

void CreateShadersAndProgram(const char* vsPath, const char* fsPath, GLuint& vertexShader, GLuint& fragmentShader, GLuint& shaderProgram)
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
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	//glUseProgram(shaderProgram);
}

void SpecifySceneVertexAttributes(GLuint shaderProgram)
{
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);	
	
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	
}

void SpecifyScreenVertexAttributes(GLuint shaderProgram)
{
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
}

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
	//Creating a VAOs before any VBO bound...
	//This vao will be enough for these program because all geometries will have the same vertex format
	GLuint vao, quadVao;
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &quadVao); // vaos created, but not vbos associated to any of them yet

	///////////////////////////////////////////////////////////////////////////////
	//Create Geometry and VBOs

	SimpleGeometryParser* parser = new SimpleGeometryParser();
	SimpleGeometry* myRectangle = parser->GetSimpleGeometry("../Resources/Geometries/Cube.geom");
	glBindBuffer(GL_ARRAY_BUFFER, myRectangle->GetHandlerVertexBuffer());
	glBufferData(GL_ARRAY_BUFFER, myRectangle->GetVerticesSize(), myRectangle->GetVertices(), GL_STATIC_DRAW);
	if (myRectangle->GetNumTriangles() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myRectangle->GetHandlerElementBuffer());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, myRectangle->GetElementsSize(), myRectangle->GetElements(), GL_STATIC_DRAW);
	}

	SimpleGeometryParser* fbParser = new SimpleGeometryParser();
	SimpleGeometry* myFrameBuffer = fbParser->GetSimpleGeometry("../Resources/Geometries/FrameBuffer.geom");
	glBindBuffer(GL_ARRAY_BUFFER, myFrameBuffer->GetHandlerVertexBuffer());
	glBufferData(GL_ARRAY_BUFFER, myFrameBuffer->GetVerticesSize(), myFrameBuffer->GetVertices(), GL_STATIC_DRAW);
	if (myFrameBuffer->GetNumTriangles() > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myFrameBuffer->GetHandlerElementBuffer());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, myFrameBuffer->GetElementsSize(), myFrameBuffer->GetElements(), GL_STATIC_DRAW);
	}

	//////////////////////////////////////////////////////////////////////////////
	//SHADERS

	GLuint vShader, fShader, program;
	GLuint quadVShader, quadFShader, quadProgram;

	CreateShadersAndProgram("../Resources/Shaders/CubeShader.vs", "../Resources/Shaders/CubeShader.fs", vShader, fShader, program);
	CreateShadersAndProgram("../Resources/Shaders/FramebufferShader.vs", "../Resources/Shaders/FramebufferShader.fs", quadVShader, quadFShader, quadProgram);

	//////////////////////////////////////////////////////////////////////////////
	//Attribute pointers
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, myRectangle->GetHandlerVertexBuffer());
	if (myRectangle->GetNumTriangles() > 0)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myRectangle->GetHandlerElementBuffer());
	SpecifySceneVertexAttributes(program);

	glBindVertexArray(quadVao);
	glBindBuffer(GL_ARRAY_BUFFER, myFrameBuffer->GetHandlerVertexBuffer());
	if (myFrameBuffer->GetNumTriangles() > 0)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myFrameBuffer->GetHandlerElementBuffer());
	SpecifyScreenVertexAttributes(quadProgram);


	//////////////////////////////////////////////////////////////////////////////
	//SCENE TEXTURES

	glUseProgram(program);															//call glUseProgram before Sending the uniform	

	//Create Textures for Scene
	Texture kittenPuppyTex;
	
	//2 steps:
	GLuint texLocation = glGetUniformLocation(program, "texKitten");
	glUniform1i(texLocation, 0); //sending the number of texture unit
	//in 1 step
	glUniform1i(glGetUniformLocation(program, "texPuppy"), 1);

	//FrameBuffer Texture Object
	glUseProgram(quadProgram);
	glUniform1i(glGetUniformLocation(quadProgram, "texFramebuffer"),0);

	//////////////////////////////////////////////////////////////////////////////
	//TRANSFORMATION MATRICES
	//01 SetConstant Matrices..
	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, -5.0f, 2.0f), //camera position
		glm::vec3(0.0f, 0.0f, -0.5f), //point of interest
		glm::vec3(0.0f, 0.0f, 1.0f)  //z is UP,    x,z is the "plane"  ground, cube is spinnning as in a wall
		);

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 1.0f, 10.0f);

	//02 Set the program before sending the matrices
	glUseProgram(program);

	//Find all of the Uniform Locations
		GLint uniModel = glGetUniformLocation(program, "model");						//Variable in Loop
		GLint uniView = glGetUniformLocation(program, "view");							//Constant
		GLint uniProj = glGetUniformLocation(program, "proj");							//Constant
		GLint uniTime = glGetUniformLocation(program, "time");							//Variable in Loop
		GLint uniTimeSin = glGetUniformLocation(program, "timeSin");					//Variable in Loop
		GLint uniOverrideColor = glGetUniformLocation(program, "overrideColor");		//Variable in Loop

	//Send Constant Matrices for now
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));


	///////////////////////////////////////////////////////////////////////////////
	//FRAMEBUFFERS STUFF
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);										//NOTE: Using Bind before attaching causes framebuffer incomplete attachment...¿?

	//ATTACHMENTS			either a GL_TEXTURE_2D, or GL_RENDERBUFFER
	//01 Color Attachment in GL_TEXTURE_2D
		GLuint texColorBuffer;
		glGenTextures(1, &texColorBuffer);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		

	//02 Depth and Stencil in a GL_RENDERBUFFER
		GLuint rboDepthStencil;
		glGenRenderbuffers(1, &rboDepthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 768);		

	//ADD ATTACHMENTS TO FRAMEBUFFER
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);			

	//Check if Framebuffer is correct
		GLuint fbStatus= glCheckFramebufferStatus(GL_FRAMEBUFFER);
		printf_s("framebuffer status: ");
		switch (fbStatus)
		{
			case GL_FRAMEBUFFER_COMPLETE: printf_s("GL_FRAMEBUFFER_COMPLETE\n"); break;
			case GL_FRAMEBUFFER_UNDEFINED: printf_s("GL_FRAMEBUFFER_UNDEFINED \n"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf_s("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT \n"); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf_s("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT \n"); break;
			case GL_FRAMEBUFFER_UNSUPPORTED: printf_s("GL_FRAMEBUFFER_UNSUPPORTED \n"); break;			
			default: printf_s("OTHER ERROR\n"); break;
		}

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
			/*if (windowEvent.type == SDL_KEYDOWN && windowEvent.key.keysym.sym == SDLK_SPACE)
			{
				speed = 180.0f;
			}	*/		
		}			
		
		/////////////////////////////////		//SET FRAMEBUFFER and Scene Stuff
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(vao);

		glEnable(GL_DEPTH_TEST);

		glUseProgram(program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, kittenPuppyTex.textures[0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, kittenPuppyTex.textures[1]);

		/////////////////////////////////		//Clear Buffers
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		//checking  PENDING
		/*unsigned char theColors[1 * 1 * 3] = { 0 };

		glReadPixels(1, 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, theColors);
		printf_s("Colors Read: %d , %d, %d\n", static_cast<int>(theColors[0]), static_cast<int>(theColors[1]), static_cast<int>(theColors[2]));*/

		
		////////////////////////////////		//Time update

		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>> (t_now - t_start).count();

		glUniform1f(uniTimeSin, (sin((double)time * 1.0f) + 1.0f) / 2.0f);
		glUniform1f(uniTime, time);

		/////////////////////////////////		//Transformation Uniforms
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(time) * 15, glm::vec3(0.0f, 0.0f, 1.0f));

		GLfloat s = sin(time * 2.0f) * 0.25f + 1.5f;
		model = glm::scale(model, glm::vec3(s, s, s)); //scaling the image with a sin function		

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		
		
		/////////////////////////////////DRAW FIRST CUBE
		int rectTris = myRectangle->GetNumTriangles();
		if ( rectTris == 0)
			glDrawArrays(GL_TRIANGLES, 0, 36);		//Drawing the vertices from vbo directly
		else
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	//6 elements or 6 vertexElements ~ 2 triangles

		/////////////////////////////////DRAW FLOOR

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);	// Pass always writting 1s
		glStencilOp(GL_KEEP,				//if stencil test fails
					GL_KEEP,				//stencil passes, depth fails
					GL_REPLACE);			//stencil passes, depth passes
		glStencilMask(0xFF);				//Write to Stencil Buffer

		glDepthMask(GL_FALSE);				//Don't write to Depth Buffer
		glClear(GL_STENCIL_BUFFER_BIT);		//Clear Stencil Buffer with 0s

		glDrawArrays(GL_TRIANGLES, 36, 6);		

		/////////////////////////////////DRAW SECOND CUBE
		glDepthMask(GL_TRUE);
		glStencilFunc(GL_EQUAL, 1, 0xFF);	//PassTest if stencil value is 1
		glStencilMask(0x00);

		model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)), glm::vec3(1, 1, -1));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glUniform3f(uniOverrideColor, 0.4f, 0.4f, 0.4f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(uniOverrideColor, 1.0f, 1.0f, 1.0f);
		
		glDisable(GL_STENCIL_TEST);

		/////////////////////////////////Default Buffer and draw the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(quadVao);

		glDisable(GL_DEPTH_TEST);
		glUseProgram(quadProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		//glBindTexture(GL_TEXTURE_2D, kittenPuppyTex.textures[0]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window);
	}	

	//Delete Objects
	delete myRectangle;
	myRectangle = NULL;

	delete parser;
	parser = NULL;

	//Delete OpenGL Objects
	glDeleteProgram(program);
	glDeleteProgram(quadProgram);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteShader(quadVShader);
	glDeleteShader(quadFShader);

	//glDeleteBuffers(1, &vbo);		//Deleted in SimpleGeometry Destructor
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &quadVao);

	//frameBufferstuff
	glDeleteTextures(1, &texColorBuffer);
	glDeleteFramebuffers(1, &frameBuffer);	
		
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	printf("bye bye");
	return 0;
}