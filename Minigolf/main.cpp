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


#include <iostream>
#include <typeinfo.h>
#include <signal.h>

//#include <vld.h>
#include <boost\shared_ptr.hpp>
#include "glm\glm.hpp"
#include "SOIL.h"
#include "lua.hpp"
#include "luabind\luabind.hpp"

#include "Utils.h"
#include "file_handling.h"
#include "shader_cache.h"
#include "entity_manager.h"
#include "render_system.h"
#include "system_manager.h"
#include "factory.h"
#include "component_type_manager.h"
#include "transform.h"
#include "camera_controller.h"
#include "input.h"
#include "time.h"
#include "ball_motor.h"
#include "physics_system.h"
#include "camera.h"
#include "gui_text_render.h"
#include "gui_text.h"
#include "gui_mesh.h"
#include "gui_mesh_render.h"

using boost::shared_ptr;

#define WINDOW_TITLE_PREFIX "Minigolf"

int CurrentWidth = 800,
	CurrentHeight = 600,
	WindowHandle = 0;

unsigned FrameCount = 0;

vector<Hole> holes;
unsigned int hole_index;

lua_State *L;

void InitOpenGL(int, char*[]);
void InitWindow(int, char*[]);
void Initialize(int, char*[]);

void ResizeFunction(int, int);
void RenderFunction(void);
void IdleFunction(void);
void TimerFunction(int);
void Destroy(void);

void MoveToHole(const unsigned int &index);

extern "C" void HandleAbort(int signal_number) {
	printf("Abort happened\n");
}

int main(int argc, char* argv[]) {
	signal(SIGABRT, &HandleAbort);
	
	InitOpenGL(argc, argv);
	Initialize(argc, argv);

	glutMainLoop();

	return 0;
}

void InitOpenGL(int argc, char* argv[]) {
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

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");
}

void Initialize(int argc, char* argv[]) {
	using boost::shared_ptr;

	// check for existing of map file in args list
	
	if (argv[1] == NULL) {
		fprintf(stderr, "Missing map file\n");
		exit(EXIT_FAILURE);
	}

	ShaderCache::AddShader("diffuse", "diffuse.vertex.2.1.glsl", "diffuse.fragment.2.1.glsl");

	shared_ptr<RenderSystem> render_system(new RenderSystem(50));
	SystemManager::AddSystem(render_system);

	shared_ptr<GuiMeshRender> gui_mesh_render(new GuiMeshRender(55));
	SystemManager::AddSystem(gui_mesh_render);

	shared_ptr<GuiTextRender> gui_text_render(new GuiTextRender(40));
	SystemManager::AddSystem(gui_text_render);

	shared_ptr<CameraController> controller(new CameraController(30));
	SystemManager::AddSystem(controller);

	shared_ptr<PhysicsSystem> physics_system(new PhysicsSystem(20));
	SystemManager::AddSystem(physics_system);

	shared_ptr<BallMotor> motor(new BallMotor(0));
	SystemManager::AddSystem(motor);

	holes = readData(argv[1]);
	hole_index = 0;
	MoveToHole(hole_index);

	/*
	Example of adding text to the screen

	EntityPtr entity = EntityManager::Create();
	GUITextPtr text(new GuiText());
	text->text = "Hello there";
	text->position = glm::vec2(400.0f, 300.0f);
	EntityManager::AddComponent(entity, text);
	*/

	// These must be called after systems and entities created
	SystemManager::Init();
	SystemManager::Resolve();
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(2, 1);

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

	EntityPtr camera = EntityManager::Find("Camera");

	if (!camera) {
		return;
	}

	CameraPtr camera_comp = EntityManager::GetComponent<Camera>(camera);
	camera_comp->aspect_ratio = CurrentWidth / (float)CurrentHeight;
}

void RenderFunction(void)
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	Time::Update();
		
	SystemManager::Update();

	if (Input::GetKeyDown("]")) {
		// todo: Switch to next level
		MoveToHole(hole_index + 1);
	} else if (Input::GetKeyDown("[")) {
		// todo: sweitch to previous level
		MoveToHole(hole_index - 1);
	}

	// warning: input update must happen after systems
	Input::Update();

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

void Destroy() {
	EntityManager::Destroy();
	ShaderCache::Destroy();
}

void MoveToHole(const unsigned int &index) {
	if (index < 0 || index >= holes.size()) {
		//fprintf(stderr, "Hole index out of bounds\n");
		//exit(EXIT_FAILURE);
		return;
	}

	// are we already at this hole?
	//if (index == hole_index) {
	//	return;
	//}

	
	hole_index = index;
	
	EntityManager::RemoveAll();

	Factory::CreateCamera(60.0f, (float)CurrentWidth / CurrentHeight, 0.1f, 1000.0f);
	Factory::CreateLevel(holes[hole_index]);
}