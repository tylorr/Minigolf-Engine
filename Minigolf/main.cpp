/*	Copyright (C) 2011 by Eddy Luten

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
*/

#include <stack>
#include <ctime>

#include "Utils.h"

#include "filehandling.h"
#include "level.h"
#include "tile_render_manager.h"

#include "shader_cache.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::value_ptr;

using namespace std;

#define WINDOW_TITLE_PREFIX "Chapter 4"

int CurrentWidth = 800,
	CurrentHeight = 600,
	WindowHandle = 0;

unsigned FrameCount = 0;

GLuint
	ProjectionMatrixUniformLocation,
	ModelViewMatrixUniformLocation,
	ShaderIds[3] = { 0 };

stack<mat4> 
	ProjectionMatrix,
	ModelViewMatrix;

GLuint
	LightPositionUniform,
	KaUniform,
	KdUniform,
	KsUniform,
	NsUniform,
	LdUniform,
	NormalMatrixUniform;

Level *level;

float CubeRotation = 0;
clock_t LastTime = 0;

clock_t previous;

bool upPressed = false;
bool downPressed = false;
bool rightPressed = false;
bool leftPressed = false;
float xAngle = 0;
float yAngle = 0;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void IdleFunction(void);
void KeyPressed(unsigned char, int, int);
void SpecialPressed(int, int, int);
void SpecialReleased(int, int, int);

void SetupShaders(void);
void DestoryShaders(void);

int main(int argc, char* argv[])
{
	Initialize(argc, argv);

	glutMainLoop();
	
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	InitWindow(argc, argv);
	
	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}
	
	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	glGetError();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");

	ModelViewMatrix.push(mat4(1.0f));
	ProjectionMatrix.push(mat4(1.0f));

	render::AddShader("diffuse", "diffuse.vertex.glsl", "diffuse.fragment.glsl");

	SetupShaders();

	//CreateCube();
	previous = clock();

	if (argv[1] == NULL)
	{
		fprintf(stderr, "Missing map file\n");
		exit(EXIT_FAILURE);
	}
	hole h = readData(argv[1]);
	level = Level::CreateLevel(h);
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);
	
	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if(WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit(EXIT_FAILURE);
	}
	
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);
	glutCloseFunc(DestoryShaders);
	glutKeyboardFunc(KeyPressed);
	glutSpecialFunc(SpecialPressed);
	glutSpecialUpFunc(SpecialReleased);
}

void KeyPressed(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:					// Escape key
		exit(EXIT_SUCCESS);
		break;
	}
}

void SpecialPressed(int Key, int X, int Y)
{
	switch (Key)
	{
	case GLUT_KEY_UP:
		upPressed = true;
		break;
	case GLUT_KEY_DOWN:
		downPressed = true;
		break;
	case GLUT_KEY_RIGHT:
		rightPressed = true;
		break;
	case GLUT_KEY_LEFT:
		leftPressed = true;
		break;
	}
}

void SpecialReleased(int Key, int X, int Y)
{
	switch (Key)
	{
	case GLUT_KEY_UP:
		upPressed = false;
		break;
	case GLUT_KEY_DOWN:
		downPressed = false;
		break;
	case GLUT_KEY_RIGHT:
		rightPressed = false;
		break;
	case GLUT_KEY_LEFT:
		leftPressed = false;
		break;
	}
}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
	
	ProjectionMatrix.top() = 
		glm::perspective(
			60.0f,
			(float)CurrentWidth / CurrentHeight,
			1.0f,
			100.0f
		);

	glUseProgram(ShaderIds[0]);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix.top()));
	glUseProgram(0);
}

void RenderFunction(void)
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	clock_t current = clock();

	float delta = float(current - previous) / CLOCKS_PER_SEC;
	previous = current;


	float keyStep = 90.0f * delta;

	if (upPressed) {
		xAngle -= keyStep;
	}
	if (downPressed) {
		xAngle += keyStep;
	}

	if (leftPressed) {
		yAngle -= keyStep;
	}

	if (rightPressed) {
		yAngle += keyStep;
	}

	ModelViewMatrix.top() = mat4(1.0f);

	// TODO: Need to make camera class
	ModelViewMatrix.top() = glm::lookAt(vec3(0, 2, 4), vec3(0, 0, 0), vec3(0, 1, 0));

	ModelViewMatrix.top() = glm::rotate(ModelViewMatrix.top(), xAngle, vec3(1, 0, 0));
	ModelViewMatrix.top() = glm::rotate(ModelViewMatrix.top(), yAngle, vec3(0, 1, 0));	

	glUseProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not use the shader program");

	// TODO: Make light class
	vec4 lightPosition = vec4(0, 5.0f, 0, 1.0f);
	lightPosition = ModelViewMatrix.top() * lightPosition; // transform light with geometry
	GLfloat Kd[] = { 0.8f, 0.8f, 0.8f }; // Light diffuse reflectivity
	GLfloat Ld[] = { 0.0f, 1.0f, 0.0f }; // Light diffuse intensity

	glUniform4fv(LightPositionUniform, 1, glm::value_ptr(lightPosition));
	glUniform3fv(KdUniform, 1, Kd);
	glUniform3fv(LdUniform, 1, Ld);
	
	// TODO: Find a better way to update and pass around mv and n matrices
	// WISHLIST: Global storage of mv matrix as well as global glPushMatrix() replica
	// TODO/WISHLIST: Scene manager
	TileRenderManager::Render(&ModelViewMatrix, ModelViewMatrixUniformLocation, NormalMatrixUniform);

		
	glUseProgram(0);

	glutSwapBuffers();
	glutPostRedisplay();
}

void IdleFunction(void)
{
	glutPostRedisplay();
}

void TimerFunction(int Value)
{
	if (0 != Value) {
		char* TempString = (char*)
			malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			FrameCount * 4,
			CurrentWidth,
			CurrentHeight
		);

		glutSetWindowTitle(TempString);
		free(TempString);
	}

	FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}



void SetupShaders()
{
	ShaderIds[0] = glCreateProgram();
	ExitOnGLError("ERROR: Could not create the shader program");
	{
		ShaderIds[1] = render::LoadShader("diffuse.fragment.glsl", GL_FRAGMENT_SHADER);
		ShaderIds[2] = render::LoadShader("diffuse.vertex.glsl", GL_VERTEX_SHADER);
		glAttachShader(ShaderIds[0], ShaderIds[1]);
		glAttachShader(ShaderIds[0], ShaderIds[2]);
	}
	glLinkProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not link the shader program");

	LightPositionUniform = glGetUniformLocation(ShaderIds[0], "LightPosition");
	KdUniform = glGetUniformLocation(ShaderIds[0], "Kd");
	LdUniform = glGetUniformLocation(ShaderIds[0], "Ld");	

	ModelViewMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ModelViewMatrix");
	ProjectionMatrixUniformLocation = glGetUniformLocation(ShaderIds[0], "ProjectionMatrix");
	NormalMatrixUniform = glGetUniformLocation(ShaderIds[0], "NormalMatrix");

	ExitOnGLError("ERROR: Could not get shader uniform locations");
}

void DestoryShaders()
{
	glDetachShader(ShaderIds[0], ShaderIds[1]);
	glDetachShader(ShaderIds[0], ShaderIds[2]);
	glDeleteShader(ShaderIds[1]);
	glDeleteShader(ShaderIds[2]);
	glDeleteProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not destroy the shaders");
}

/*
void DrawCube(void)
{
	float CubeAngle;
	clock_t Now = clock();

	if (LastTime == 0)
		LastTime = Now;

	CubeRotation += 45.0f * ((float)(Now - LastTime) / CLOCKS_PER_SEC);
	CubeAngle = DegreesToRadians(CubeRotation);
	LastTime = Now;

	ModelViewMatrix.push(ModelViewMatrix.top());

	//ModelViewMatrix.top() = rotate(ModelViewMatrix.top(), CubeRotation, vec3(0, 1.0f, 0));
	//ModelViewMatrix.top() = rotate(ModelViewMatrix.top(), CubeRotation, vec3(1.0f, 0, 0));

	glUseProgram(ShaderIds[0]);
	ExitOnGLError("ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelViewMatrixUniformLocation, 1, GL_FALSE, value_ptr(ModelViewMatrix.top()));
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	ExitOnGLError("ERROR: Could not draw the cube");

	glBindVertexArray(0);
	glUseProgram(0);

	ModelViewMatrix.pop();
}
*/