#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	state = "PLAYING";

	current_music_track = 1;

	enemy_body = nullptr;
	enemy_body_2 = nullptr;
	hinge = nullptr;
	enemy_cube = Cube(2.0f, 2.0f, 2.0f);
	enemy_cube_2 = Cube(2.0f, 2.0f, 2.0f);

	vertical_enemy_body = nullptr;
	vertical_enemy_body_2 = nullptr;
	vertical_hinge = nullptr;
	vertical_enemy_cube = Cube(2.0f, 2.0f, 2.0f);
	vertical_enemy_cube_2 = Cube(2.0f, 2.0f, 2.0f);

	sphere_body = nullptr;
	sphere_body_2 = nullptr;
	sphere_hinge = nullptr;
	sphere_cube = Cube(2.0f, 2.0f, 2.0f);
	enemy_sphere = Sphere(2.0f);

	/* SENSORS */
	starting_lap_sensor_cube.size = vec3(14.0f, 2.0f, 2.0f);
	starting_lap_sensor_cube.SetPos(0.0f, 1.0f, 0.0f);
	starting_lap_sensor_pbody = nullptr;
	ending_lap_sensor_cube.size = vec3(14.0f, 2.0f, 2.0f);
	ending_lap_sensor_cube.SetPos(0.0f, 1.0f, 60.5f);
	ending_lap_sensor_pbody = nullptr;

	/* MAP MAIN PLANE */
	plane.normal = vec3(0.0f, 1.0f, 0.0f);
	plane.constant = 0.0f;
	
	current_time_seconds = 0;
	time_to_beat_seconds = 36;

	current_lap = 0;
	can_end = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//-----
	/* ADD BODIES */
	left_main_border = CreateCube(vec3(8.0f, 1.0f, 0.0f), vec3(2.0f, 2.0f, 60.0f));
	right_main_border = CreateCube(vec3(-8.0f, 1.0f, 0.0f), vec3(2.0f, 2.0f, 60.0f));
	goal_left_pilar = CreateCube(vec3(-10.0f, 5.0f, 0.0f), vec3(0.5f, 10.0f, 0.5f));
	goal_right_pilar = CreateCube(vec3(10.0f, 5.0f, 0.0f), vec3(0.5f, 10.0f, 0.5f));
	goal_top_beam = CreateCube(vec3(0.0f, 9.5f, 0.0f), vec3(19.5f, 1.0f, 0.5f));

	main_climb_start = CreateCube(vec3(0.0f, 1.0f, 32.0f), vec3(18.0f, 0.25f, 6.0f), 0.0f, White, -25.0f, { 1.0f, 0.0f, 0.0f });
	main_climb_finish = CreateCube(vec3(0.0f, 1.0f, 48.0f), vec3(18.0f, 0.25f, 6.0f), 0.0f, White, 25.0f, { 1.0f, 0.0f, 0.0f });

	left_main_climb_border = CreateCube(vec3(8.0f, 1.0f, 60.0f), vec3(2.0f, 2.0f, 20.0f));
	right_main_climb_border = CreateCube(vec3(-8.0f, 1.0f, 65.0f), vec3(2.0f, 2.0f, 30.0f));

	left_first_turn = CreateCube(vec3(19.0f, 1.0f, 69.0f), vec3(2.0f, 2.0f, 20.0f), 0.0f, White, 90.0f, { 0.0f, 1.0f, 0.0f });
	right_first_turn = CreateCube(vec3(15.5f, 1.0f, 79.0f), vec3(2.0f, 2.0f, 45.0f), 0.0f, White, 90.0f, { 0.0f, 1.0f, 0.0f });
	
	left_second_turn = CreateCube(vec3(37.0f, 1.0f, 67.5f), vec3(2.0f, 2.0f, 21.0f));
	right_second_turn = CreateCube(vec3(30.0f, 1.0f, 67.5f), vec3(2.0f, 2.0f, 5.0f));

	left_third_turn = CreateCube(vec3(29.0f, 1.0f, 58.0f), vec3(14.0f, 2.0f, 2.0f));
	right_third_turn = CreateCube(vec3(26.5f, 1.0f, 64.0f), vec3(9.0f, 2.0f, 2.0f));

	initial_bridge_ramp = CreateCube(vec3(18.0f, 2.0f, 61.0f), vec3(10.0f, 0.25f, 4.0f), 0.0f, White, -25.0f, { 0, 0, 1 });
	middle_bridge_platform = CreateCube(vec3(0.0f, 4.1f, 61.0f), vec3(27.0f, 0.25f, 4.0f));
	//final_bridge_ramp = CreateCube(vec3(-18.0f, 2.0f, 61.0f), vec3(10.0f, 0.25f, 4.0f), 0.0f, White, 25.0f, { 0, 0, 1 });
	
	left_fourth_turn = CreateCube(vec3(-29.0f, 1.0f, 64.0f), vec3(14.0f, 2.0f, 2.0f));
	right_fourth_turn = CreateCube(vec3(-26.0f, 1.0f, 58.0f), vec3(8.0f, 2.0f, 2.0f));

	left_large_straight = CreateCube(vec3(-29.0f, 1.0f, -16.5f), vec3(2.0f, 2.0f, 150.0f));
	right_large_straight = CreateCube(vec3(-37.0f, 1.0f, -14.0f), vec3(2.0f, 2.0f, 169.0f));

	left_large_straight_wall = CreateCube(vec3(-29.0f, 4.5f, -10.5f), vec3(2.0f, 5.0f, 150.0f));
	right_large_straight_wall = CreateCube(vec3(-37.0f, 4.5f, -10.5f), vec3(2.0f, 5.0f, 150.0f));

	large_straight_roof = CreateCube(vec3(-33.0f, 8.0f, 12.5f), vec3(10.0f, 2.0f, 89.0f));

	main_climb_start2 = CreateCube(vec3(-33.0f, 3.4f, -75.0f), vec3(7.0f, 0.25f, 66.0f), 0.0f, White, -06.0f, { 1.0f, 0.0f, 0.0f });
	main_climb_finish2 = CreateCube(vec3(-33.0f, 3.4f, -35.0f), vec3(7.0f, 0.25f, 16.0f), 0.0f, White, 25.0f, { 1.0f, 0.0f, 0.0f });

	//left_fifth_turn = CreateCube(vec3(-17.5f, 1.0f, -31.0f), vec3(21.0f, 2.0f, 2.0f));
	//right_fifth_turn = CreateCube(vec3(-14.5f, 1.0f, -42.0f), vec3(43.0f, 2.0f, 2.0f));

	final_closing_straight = CreateCube(vec3(8.0f, 1.0f, -36.5f), vec3(2.0f, 2.0f, 13.0f));
	//-----

	sphere_cube.SetPos(-33.0f, 13.5f, -60.0f);
	sphere_cube.color = Black;
	sphere_body = App->physics->AddBody(sphere_cube, 0.0f); // We need this enemy_body pointer, in this case.

	enemy_sphere.SetPos(-33.0f, 7.5f, -60.0f);
	enemy_sphere.color = Red;
	enemy_sphere.radius = 2.0f;
	sphere_body_2 = App->physics->AddBody(enemy_sphere, 1.0f); // We need this enemy_body pointer, in this case.

	sphere_hinge = App->physics->AddConstraintHinge(*sphere_body, *sphere_body_2, vec3(0, 0, 0), vec3(0, 6, 0), vec3(1, 0, 0), vec3(0, 0, 0), true);
	sphere_hinge->enableAngularMotor(true, 3.0f, INFINITE);

	//-----
	/* HINGE */
	enemy_cube.SetPos(0.0f, 2.0f, 6.0f);
	enemy_cube.color = Black;
	enemy_body = App->physics->AddBody(enemy_cube, 0.0f); // We need this enemy_body pointer, in this case.

	enemy_cube_2.SetPos(0.0f, 2.0f, 10.0f);
	enemy_cube_2.color = Red;
	enemy_body_2 = App->physics->AddBody(enemy_cube_2, 1.0f); // We need this enemy_body pointer, in this case.

	hinge = App->physics->AddConstraintHinge(*enemy_body, *enemy_body_2, vec3(0, 0, 0), vec3(0, 0, 4), vec3(0, 1, 0), vec3(0, 0, 0), true);
	hinge->enableAngularMotor(true, 2.0f, INFINITE);

	/* VERTICAL HINGE */
	vertical_enemy_cube.SetPos(0.0f, 10.0f, 40.0f);
	vertical_enemy_cube.color = Black;
	vertical_enemy_body = App->physics->AddBody(vertical_enemy_cube, 0.0f); // We need this enemy_body pointer, in this case.

	vertical_enemy_cube_2.SetPos(0.0f, 16.0f, 40.0f);
	vertical_enemy_cube_2.color = Red;
	vertical_enemy_body_2 = App->physics->AddBody(vertical_enemy_cube_2, 1.0f); // We need this enemy_body pointer, in this case.

	vertical_hinge = App->physics->AddConstraintHinge(*vertical_enemy_body, *vertical_enemy_body_2, vec3(0, 0, 0), vec3(0, 6, 0), vec3(0, 0, 1), vec3(0, 0, 0), true);
	vertical_hinge->enableAngularMotor(true, 2.0f, INFINITE);
	//-----

	/* SENSORS */
	starting_lap_sensor_pbody = App->physics->AddBody(starting_lap_sensor_cube, 0.0f);
	starting_lap_sensor_pbody->SetAsSensor(true);
	starting_lap_sensor_pbody->collision_listeners.add(this);

	ending_lap_sensor_pbody = App->physics->AddBody(ending_lap_sensor_cube, 0.0f);
	ending_lap_sensor_pbody->SetAsSensor(true);
	ending_lap_sensor_pbody->collision_listeners.add(this);
	
	/* MAP OBSTACLES */
	obstacle_01 = CreateCylinder({ 17, 1, 75 }, 1.5f, 2.0f, 0.0f, Green, 90.0f, { 0, 0, 1 });
	obstacle_02 = CreateCylinder({ -31.8f, 4.0f, 0 }, 1.15f, 8.0f, 0.0f, Green, 90.0f, { 0, 0, 1 });
	obstacle_03 = CreateCylinder({ -34.2f, 4.0f, 16.0f }, 1.15f, 8.0f, 0.0f, Green, 90.0f, { 0, 0, 1 });
	obstacle_04 = CreateCylinder({ -31.8f, 4.0f, 32.0f }, 1.15f, 8.0f, 0.0f, Green, 90.0f, { 0, 0, 1 });
	obstacle_05 = CreateCylinder({ -34.2f, 4.0f, -16.0f }, 1.15f, 8.0f, 0.0f, Green, 90.0f, { 0, 0, 1 });
	obstacle_06 = CreateCylinder({ -16, 2.0f, -38.0f }, 1.15f, 4.0f, 0.0f, Green, 90.0f, { 0, 0, 1 });
	obstacle_07 = CreateCylinder({ -4, 2.0f, -32.0f }, 1.15f, 4.0f, 0.0f, Green, 90.0f, { 0, 0, 1 });

	// ---------
	track_01 = "Assets/Music/Naoki_Naotyu-SpeedWorld.ogg";
	track_02 = "Assets/Music/Initial_D-Deja_Vu.ogg";

	App->audio->PlayMusic(track_01.GetString());

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	game_timer.Start();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	// Delete lists
	cubes.clear();
	cylinders.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	/* TIMING */
	if (current_lap > 0)
	{
		time_played_minutes_s.create("%i", (game_timer.Read() / 1000) / 60);
		time_played_seconds_s.create("%i", (game_timer.Read() / 1000) % 60);
		current_time_seconds = game_timer.Read() / 1000;
	}

	/* INPUT */
	// Select current song
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		current_music_track = 1;
		App->audio->PlayMusic(track_01.GetString());
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		current_music_track = 2;
		App->audio->PlayMusic(track_02.GetString());
	}

	// Restart game
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		ResetGame();
	}


	/* RENDER MAIN PLANE */
	plane.Render();

	//-----

	sphere_cube.Render();
	enemy_sphere.Render();

	mat4x4 transform3;
	sphere_body_2->GetTransform(transform3.M);
	enemy_sphere.transform = transform3;

	/* RENDER HINGE ELEMENTS */
	enemy_cube.Render();
	enemy_cube_2.Render();

	mat4x4 transform;
	enemy_body_2->GetTransform(transform.M);
	enemy_cube_2.transform = transform;

	/* RENDER VERTICAL HINGE ELEMENTS */
	vertical_enemy_cube.Render();
	vertical_enemy_cube_2.Render();

	mat4x4 transform2;
	vertical_enemy_body_2->GetTransform(transform2.M);
	vertical_enemy_cube_2.transform = transform2;
	//-----

	/* RENDER PRIMITIVES */
	for (p2List_item<Cube>* cube_item = cubes.getFirst(); cube_item != nullptr; cube_item = cube_item->next)
	{
		cube_item->data.Render();
	}

	for (p2List_item<Cylinder>* cylinder_item = cylinders.getFirst(); cylinder_item != nullptr; cylinder_item = cylinder_item->next)
	{
		cylinder_item->data.Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1 == starting_lap_sensor_pbody && body2 == App->player->vehicle)
	{
		if (!can_end)
		{
			game_timer.Start();
			current_lap = 1;
		}
		else if (can_end)
		{
			if (current_time_seconds < time_to_beat_seconds)
			{
				state = "YOU WIN! Press 'R' to restart.";
			}
			else if (current_time_seconds > time_to_beat_seconds)
			{
				state = "YOU LOSE! Press 'R' to restart.";
			}
			game_timer.Stop();
		}
	}

	if (body1 == ending_lap_sensor_pbody && body2 == App->player->vehicle && current_lap > 0)
	{
		can_end = true;
	}
}

void ModuleSceneIntro::ResetGame()
{
	state = "PLAYING";
	current_lap = 0;
	can_end = false;
	current_time_seconds = 0;
	time_played_minutes_s = "";
	time_played_seconds_s = "";
	current_time_seconds = 0;
	App->player->vehicle->GetBody()->setLinearVelocity(btVector3(0, 0, 0));
	App->player->vehicle->GetBody()->setAngularVelocity(btVector3(0, 0, 0));
	App->player->vehicle->SetTransform(IdentityMatrix.M);
	App->player->turn = App->player->acceleration = App->player->brake = 0.0f;
	App->player->SetCurrentGear(1);
	App->player->SetPosition(vec3(App->player->starting_position));
	App->player->vehicle->SetPos(App->player->GetPosition().x, App->player->GetPosition().y, App->player->GetPosition().z);
}


int ModuleSceneIntro::GetCurrentMusicTrack() const
{
	return current_music_track;
}

// Note: if the mass is 0.0f, we don't need to add the collision listener. We suppose in this function that
// the mass will be 0.0f as default. It won't work otherwise. If we want to update the collider,
// we need to crate the PhysBody3D* manually.
Cube ModuleSceneIntro::CreateCube(vec3 position, vec3 size, float mass, Color color, float angle, vec3 angle_rot, bool is_collider)
{
	Cube cube(size.x, size.y, size.z);
	cube.SetPos(position.x, position.y, position.z);
	cube.color = color;

	if (angle != 0.0f)
	{
		cube.SetRotation(angle, angle_rot);
	}

	if (is_collider)
	{
		App->physics->AddBody(cube, mass); 
	}

	cubes.add(cube);

	return cube;
}

Cylinder ModuleSceneIntro::CreateCylinder(vec3 position, float radius, float height, float mass, Color color, float angle, vec3 angle_rot, bool is_collider)
{
	Cylinder cylinder(radius, height);
	cylinder.SetPos(position.x, position.y, position.z);
	cylinder.color = color;

	if (angle != 0.0f)
	{
		cylinder.SetRotation(angle, angle_rot);
	}

	if (is_collider)
	{
		App->physics->AddBody(cylinder, mass);
	}

	cylinders.add(cylinder);

	return cylinder;
}
