/*	
	Permission for use of shader code:

	Copyright (C) 2011 by Eddy Luten

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

//#include <vld.h>
#include <ctime>

#include <boost\shared_ptr.hpp>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\string_cast.hpp"

#include "Utils.h"
#include "file_handling.h"
#include "level.h"
#include "shader_cache.h"
#include "component_type.h"
#include "entity_manager.h"
#include "render_system.h"
#include "system_manager.h"
#include "factory.h"
#include "component_type_manager.h"
#include "transform.h"
#include "camera_controller.h"
#include "input.h"

using boost::shared_ptr;
using boost::dynamic_pointer_cast;

using glm::quat;

#define WINDOW_TITLE_PREFIX "Chapter 4"

int CurrentWidth = 800,
	CurrentHeight = 600,
	WindowHandle = 0;

unsigned FrameCount = 0;

clock_t previous;

shared_ptr<Transform> ball_transform;
shared_ptr<RenderSystem> render_system;
shared_ptr<Transform> camera_transform;

void Initialize(int, char*[]);
void InitWindow(int, char*[]);

void ResizeFunction(int, int);
void RenderFunction(void);
void IdleFunction(void);

void TimerFunction(int);

void Destroy(void);

int main(int argc, char* argv[])
{
	Initialize(argc, argv);

	glutMainLoop();

	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	// check for existing of map file in args list
	
	if (argv[1] == NULL)
	{
		fprintf(stderr, "Missing map file\n");
		exit(EXIT_FAILURE);
	}

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

	// culling disabled so we can see the bottom of tiles
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");
	

	//--------------------------------------------------------------------------
	// Setup world

	ShaderCache::AddShader("diffuse", "diffuse.vertex.2.1.glsl", "diffuse.fragment.2.1.glsl");

	vec3 reference = vec3(0, 3.0f, 3.0f);

	render_system = shared_ptr<RenderSystem>(new RenderSystem(false, reference, vec3(0, 1, 0)));
	SystemManager::AddSystem(render_system);
	shared_ptr<CameraController> controller(new CameraController());
	SystemManager::AddSystem(controller);

	Factory::CreateCamera(60.0f, (float)CurrentWidth / CurrentHeight, 0.1f, 1000.0f);

	Hole h = readData(argv[1]);
	Factory::CreateLevel(h);

	controller->Resolve();

	shared_ptr<Entity> camera = EntityManager::Find("Camera");
	//camera_transform = boost::dynamic_pointer_cast<Transform>(EntityManager::GetComponent(camera, "Transform"));
	camera_transform = EntityManager::GetComponent<Transform>(camera, "Transform");

	shared_ptr<Entity> ball = EntityManager::Find("Ball");
	ball_transform = EntityManager::GetComponent<Transform>(ball, "Transform");//boost::dynamic_pointer_cast<Transform>(EntityManager::GetComponent(ball, "Transform"));
	//camera_transform->parent = ball_transform;

	previous = clock();
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(2, 1);
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
	glutCloseFunc(Destroy);
	glutKeyboardFunc(Input::KeyPressed);
	glutKeyboardUpFunc(Input::KeyReleased);
	glutSpecialFunc(Input::SpecialPressed);
	glutSpecialUpFunc(Input::SpecialReleased);
}

void KeyPressed(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:					// Escape key
		glutLeaveMainLoop();
		break;
	}
}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
	
	// todo: move this logic into camera controller
	/*
	ProjectionMatrix.top() = 
		glm::perspective(
			60.0f,
			(float)CurrentWidth / CurrentHeight,
			1.0f,
			100.0f
		);
	*/
}

void RenderFunction(void)
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	clock_t current = clock();
	float delta = float(current - previous) / CLOCKS_PER_SEC;
	previous = current;

	float keyStep = delta;
	
	if (Input::GetKey("w")) {
		ball_transform->Translate(0, 0, -keyStep);
	}

	if (Input::GetKey("s")) {
		ball_transform->Translate(0, 0, keyStep);
	}

	if (Input::GetKey("a")) {
		ball_transform->Translate(-keyStep, 0, 0);
	}

	if (Input::GetKey("d")) {
		ball_transform->Translate(keyStep, 0, 0);
	}
	

	SystemManager::Update();

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

void Destroy()
{
	ShaderCache::Destroy();
}