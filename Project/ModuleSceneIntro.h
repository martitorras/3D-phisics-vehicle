#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2SString.h"
#include "Color.h"
#include "Timer.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// #define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	int GetCurrentMusicTrack() const;

	Cube CreateCube(vec3 position = { 0.0f, 0.0f, 0.0f }, vec3 size = { 0.0f, 0.0f, 0.0f }, float mass = 0.0f, Color color = White,
		float angle = 0.0f, vec3 angle_rot = { 0.0f, 0.0f, 0.0f }, bool is_collider = true);

	Cylinder CreateCylinder(vec3 position = { 0.0f,0.0f,0.0f }, float radius = 1.0f, float height = 1.0f, float mass = 0.0f, Color color = White,
		float angle = 0.0f, vec3 angle_rot = { 0.0f, 0.0f, 0.0f }, bool is_collider = true);

public:
	/*PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];*/

	/* MAP OBJECTS */
	Plane plane;

	Cube left_main_border;
	Cube right_main_border;
	
	Cube goal_left_pilar;
	Cube goal_right_pilar;
	Cube goal_top_beam;

	Cube main_climb_start;
	Cube main_climb_finish;

	Cube left_main_climb_border;
	Cube right_main_climb_border;

	Cube left_first_turn;
	Cube right_first_turn;

	Cube left_second_turn;
	Cube right_second_turn;

	Cube left_third_turn;
	Cube right_third_turn;

	Cube initial_bridge_ramp;
	Cube middle_bridge_platform;
	Cube final_bridge_ramp;

	Cube left_fourth_turn;
	Cube right_fourth_turn;

	Cube left_large_straight;
	Cube right_large_straight;

	Cube left_fifth_turn;
	Cube right_fifth_turn;
	
	Cube final_closing_straight;

	/* HORIZONTAL HINGE */
	PhysBody3D* enemy_body;
	Cube enemy_cube;
	PhysBody3D* enemy_body_2;
	Cube enemy_cube_2;
	btHingeConstraint* hinge;

	/* VERTICAL HINGE */
	PhysBody3D* vertical_enemy_body;
	Cube vertical_enemy_cube;
	PhysBody3D* vertical_enemy_body_2;
	Cube vertical_enemy_cube_2;
	btHingeConstraint* vertical_hinge;

	/* MAP OBSTACLES */
	Cylinder obstacle_01;

	/* STORE PRIMITIVES SO WE CAN BLIT A LOT MORE EASILY */
	p2List<Cube> cubes;
	p2List<Cylinder> cylinders;

	// Music
	int current_music_track;
	p2SString track_01;
	p2SString track_02;

	// Timing
	Timer game_timer;
	p2SString time_played_minutes_s;
	p2SString time_played_seconds_s;

	int current_time_seconds;
	int time_to_beat_seconds;
};
