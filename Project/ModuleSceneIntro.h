#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "p2SString.h"
#include "Color.h"

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

	/* STORE PRIMITIVES SO WE CAN BLIT A LOT MORE EASILY */
	p2List<Cube> cubes;

	// Music
	int current_music_track;
	p2SString track_01;
	p2SString track_02;
};
