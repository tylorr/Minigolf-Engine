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
#include "script.h"
#include "script_system.h"
#include "ball_motor.h"
#include "ball_component.h"

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
void MakeHUD(void);

extern "C" void HandleAbort(int signal_number) {
	printf("Abort happened\n");
}

template <typename T>
void Bind() {
	T::Bind(L);
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

	glClearColor(0.529f, 0.808f, 0.98f, 1.0f);

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

	//---------------------------------------------------------------------
	// Initialize lua

	L = luaL_newstate();
	luaL_openlibs(L);
	luabind::open(L);

	Bind<Entity>();

	//---------------------------------------------------------------------
	// Initialize systems

	shared_ptr<ScriptSystem> script_system(new ScriptSystem(L, 10));
	SystemManager::AddSystem(script_system);

	shared_ptr<RenderSystem> render_system(new RenderSystem(50));
	SystemManager::AddSystem(render_system);

	shared_ptr<GuiMeshRender> gui_mesh_render(new GuiMeshRender(55));
	SystemManager::AddSystem(gui_mesh_render);

	shared_ptr<GuiTextRender> gui_text_render(new GuiTextRender(40));
	SystemManager::AddSystem(gui_text_render);

	shared_ptr<CameraController> controller(new CameraController(30, "camera_config.lua"));
	SystemManager::AddSystem(controller);

	shared_ptr<PhysicsSystem> physics_system(new PhysicsSystem(20, "physics_config.lua"));
	SystemManager::AddSystem(physics_system);

	shared_ptr<BallMotor> motor(new BallMotor(0, "ball_motor_config.lua"));
	SystemManager::AddSystem(motor);

	//---------------------------------------------------------------------
	// Entities and Components

	ShaderCache::AddShader("diffuse", "diffuse.vertex.2.1.glsl", "diffuse.fragment.2.1.glsl");

	holes = readData(argv[1]);
	hole_index = 0;
	MoveToHole(hole_index);

	/*
	// Example logic script
	EntityPtr entity = EntityManager::Create();
	ScriptPtr script(new Script("test.lua"));
	EntityManager::AddComponent(entity, script);
	*/

	/*
	// Example of adding text to the screen
	EntityPtr t = EntityManager::Create();
	GUITextPtr text(new GuiText());
	text->text = "Hello there";
	text->position = glm::vec2(400.0f, 300.0f);
	EntityManager::AddComponent(t, text);
	*/

	//---------------------------------------------------------------------
	// Finalize initialization

	// These must be called after systems and entities created
	SystemManager::Init();
	SystemManager::ReloadScript();
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

	if (Input::GetKeyDown("r")) {
		SystemManager::ReloadScript();
	}
		
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

void MoveToHole(const unsigned int &index) {
	shared_ptr<BallMotor> motor(new BallMotor(0));

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
	MakeHUD();
	motor->Init();
}

void Destroy() {
	EntityManager::Destroy();
	ShaderCache::Destroy();
	lua_close(L);
}

void MakeHUD(){
	EntityPtr hud_power = EntityManager::Create();
	GUITextPtr text(new GuiText());
	text->text = "POWER";
	text->position = glm::vec2(700.0f, 385.0f);
	EntityManager::AddComponent(hud_power, text);

	EntityPtr hud_lower = EntityManager::Create();
	GUITextPtr lower(new GuiText());
	lower->text = "_____";
	lower->position = glm::vec2(715.0f, 350.0f);
	EntityManager::AddComponent(hud_lower, lower);
	EntityManager::Register(hud_lower, "Lower");

	EntityPtr hud_upper = EntityManager::Create();
	GUITextPtr upper(new GuiText());
	upper->text = "_____";
	upper->position = glm::vec2(715.0f, 100.0f);
	EntityManager::AddComponent(hud_upper, upper);
	EntityManager::Register(hud_upper, "Upper");

	EntityPtr hud_par_text = EntityManager::Create();
	GUITextPtr par_text(new GuiText());
	par_text->text = "PAR";
	par_text->position = glm::vec2(100.0f, 500.0f);
	EntityManager::AddComponent(hud_par_text, par_text);

	EntityPtr hud_par_num = EntityManager::Create();
	GUITextPtr par_num(new GuiText());
	par_num->text = holes[hole_index].par;
	par_num->position = glm::vec2(200.0f, 500.0f);
	EntityManager::AddComponent(hud_par_num, par_num);

	EntityPtr hud_strokes_text = EntityManager::Create();
	GUITextPtr strokes_text(new GuiText());
	strokes_text->text = "STROKES";
	strokes_text->position = glm::vec2(50.0f, 450.0f);
	EntityManager::AddComponent(hud_strokes_text, strokes_text);

	EntityPtr hud_strokes_num = EntityManager::Create();
	GUITextPtr strokes_num(new GuiText());
	strokes_num->text = "0";
	strokes_num->position = glm::vec2(200.0f, 450.0f);
	EntityManager::AddComponent(hud_strokes_num, strokes_num);
	EntityManager::Register(hud_strokes_num, "Strokes");

	EntityPtr hud_power_bar = EntityManager::Create();
	GUITextPtr power_bar(new GuiText());
	power_bar->text = "==";
	power_bar->position = glm::vec2(730.0f, 350.0f);
	EntityManager::AddComponent(hud_power_bar, power_bar);
	EntityManager::Register(hud_power_bar, "Power Bar");

	EntityPtr hud_hole_name = EntityManager::Create();
	GUITextPtr hole_name(new GuiText());
	char buffer[65];
	_itoa_s(hole_index + 1,buffer,65,10);
	hole_name->text = "Hole " + (string)buffer +":  " + holes[hole_index].name;
	hole_name->position = glm::vec2(50.0f, 50.0f);
	EntityManager::AddComponent(hud_hole_name, hole_name);

	EntityPtr hud_score = EntityManager::Create();
	GUITextPtr score_text(new GuiText());
	score_text->text = "Score";
	score_text->position = glm::vec2(350.0f, 550.0f);
	EntityManager::AddComponent(hud_score, score_text);
	EntityManager::Register(hud_power_bar, "Power Bar");

	EntityPtr hud_score_num = EntityManager::Create();
	GUITextPtr score_num(new GuiText());
	score_num->text = "0";
	score_num->position = glm::vec2(450.0f, 550.0f);
	EntityManager::AddComponent(hud_score_num, score_num);
	EntityManager::Register(hud_score_num, "Score");

	EntityPtr hud_message = EntityManager::Create();
	GUITextPtr message(new GuiText());
	message->text = " ";
	message->position = glm::vec2(325.0f, 150.0f);
	EntityManager::AddComponent(hud_message, message);
	EntityManager::Register(hud_message, "Message");

}
