#include "ball_motor.h"
#include "entity.h"
#include "entity_manager.h"
#include "transform.h"
#include "time.h"
#include "input.h"
#include "ball_component.h"
#include "gui_text_render.h"
#include "gui_text.h"

using boost::shared_ptr;

float power = 0.0f;
bool increasing = true;
int num_strokes = 0;
int total_score = 0;



BallMotor::BallMotor(const int &layer) : EntitySystem(layer) { }
BallMotor::BallMotor(const int &layer, const std::string &script) : EntitySystem(layer, script) { }

void BallMotor::Init(){
	num_strokes = 0;
	power = 0.f;
	increasing = true;
}

void BallMotor::ReloadScript() {
	EntitySystem::ReloadScript();

	max_speed_ = GetAttribute<float>("max_speed");
	speed_step_ = GetAttribute<float>("speed_step");
}

void BallMotor::Process() {
	EntityPtr ball_ = EntityManager::Find("Ball");

	if (!ball_) {
		return;
	}

	TransformPtr ball_transform = EntityManager::GetComponent<Transform>(ball_);
	BallComponentPtr ball_comp = EntityManager::GetComponent<BallComponent>(ball_);

	float delta = Time::GetDeltaTime();
	float speed = -10.0f;
	float rot_speed = 50.0f;

	// TODO: replace this later with proper force/acceleration input
	//vec3 velocity;
	
	ball_comp->acceleration = vec3();

	//Grab and set new values for the strokes and score HUD elements
	EntityPtr hud_strokes = EntityManager::Find("Strokes");
	GUITextPtr strokes = EntityManager::GetComponent<GuiText>(hud_strokes);
	char buffer[65];
	_itoa_s(num_strokes,buffer,65,10);
	strokes->text = buffer;

	EntityPtr hud_score = EntityManager::Find("Score");
	GUITextPtr score = EntityManager::GetComponent<GuiText>(hud_score);
	char buffer2[65];
	_itoa_s(total_score,buffer2,65,10);
	score->text = buffer2;

	/*
	if (Input::GetKey("up")) {
		ball_comp->acceleration += vec3(0, 0, speed);
	}
	if (Input::GetKey("down")) {
		ball_comp->acceleration += vec3(0, 0, -speed);
	}
	if (Input::GetKey("left")) {
		ball_comp->acceleration += vec3(speed, 0, 0);
	}
	if (Input::GetKey("right")) {
		ball_comp->acceleration += vec3(-speed, 0, 0);
	}
	*/
	
	static const float epsilon = 0.01f;

	if (glm::length(ball_comp->velocity) < epsilon) {

		EntityPtr power_bar = EntityManager::Find("Power Bar");
		GUITextPtr bar = gui_text_mapper_(power_bar);

		EntityPtr upper = EntityManager::Find("Upper");
		GUITextPtr upper_text = gui_text_mapper_(upper);

		EntityPtr lower = EntityManager::Find("Lower");
		GUITextPtr lower_text = gui_text_mapper_(lower);

		float l = lower_text->position.y;
		float u = upper_text->position.y + 19;

		float diff = l - u;
		
		//bar->position.y = 350.f - (power * 22.f);
		bar->position.y = l - (diff * power);

		if (Input::GetKey("left")) {
			ball_transform->Rotate(vec3(0, 1, 0), rot_speed * delta);
		}

		if (Input::GetKey("right")) {
			ball_transform->Rotate(vec3(0, 1, 0), -rot_speed * delta);
		}

		if (Input::GetKey("t")) {
			if(increasing){
				power += speed_step_ * delta;
				if (power > 1.0f) { 
					power = 1.0f;
					increasing = false; 
				}
			}
			else{
				//EntityManager::Remove(hud_power_elements[(int)power]);
				power -= speed_step_ * delta;
				if (power < 0) { 
					power = 0; 
					increasing = true;
				}
			}
		}

		if (Input::GetKeyUp("t")) {
			ball_comp->velocity += ball_transform->forward() * (-power * max_speed_);
			num_strokes++;
			total_score++;
			power = 0.f;
		}
	
	}
	
}