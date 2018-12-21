#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	/* MAP MAIN PLANE */
	plane.normal = vec3(0.0f, 1.0f, 0.0f);
	plane.constant = 0.0f;
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
	final_bridge_ramp = CreateCube(vec3(-18.0f, 2.0f, 61.0f), vec3(10.0f, 0.25f, 4.0f), 0.0f, White, 25.0f, { 0, 0, 1 });
	
	left_fourth_turn = CreateCube(vec3(-29.0f, 1.0f, 64.0f), vec3(14.0f, 2.0f, 2.0f));
	right_fourth_turn = CreateCube(vec3(-26.0f, 1.0f, 58.0f), vec3(8.0f, 2.0f, 2.0f));

	//-----

	//-----
	/* HINGE */
	enemy_cube.SetPos(0.0f, 2.0f, 6.0f);
	enemy_cube.color = Red;
	enemy_body = App->physics->AddBody(enemy_cube, 0.0f); // We need this enemy_body pointer, in this case.

	enemy_cube_2.SetPos(0.0f, 2.0f, 10.0f);
	enemy_cube_2.color = Black;
	enemy_body_2 = App->physics->AddBody(enemy_cube_2, 1.0f); // We need this enemy_body pointer, in this case.

	hinge = App->physics->AddConstraintHinge(*enemy_body, *enemy_body_2, vec3(0, 0, 0), vec3(0, 0, 4), vec3(0, 1, 0), vec3(0, 0, 0), true);
	hinge->enableAngularMotor(true, 2.0f, INFINITE);

	/* VERTICAL HINGE */
	vertical_enemy_cube.SetPos(0.0f, 10.0f, 40.0f);
	vertical_enemy_cube.color = Red;
	vertical_enemy_body = App->physics->AddBody(vertical_enemy_cube, 0.0f); // We need this enemy_body pointer, in this case.

	vertical_enemy_cube_2.SetPos(0.0f, 16.0f, 40.0f);
	vertical_enemy_cube_2.color = Black;
	vertical_enemy_body_2 = App->physics->AddBody(vertical_enemy_cube_2, 1.0f); // We need this enemy_body pointer, in this case.

	vertical_hinge = App->physics->AddConstraintHinge(*vertical_enemy_body, *vertical_enemy_body_2, vec3(0, 0, 0), vec3(0, 6, 0), vec3(0, 0, 1), vec3(0, 0, 0), true);
	vertical_hinge->enableAngularMotor(true, 2.0f, INFINITE);
	//-----
	
	/* MAP OBSTACLES */
	obstacle_01 = CreateCylinder({ 17, 1, 75 }, 1.5f, 2.0f, 0.0f, Green, 90.0f, {0, 0, 1});

	// ---------
	track_01 = "Assets/Music/Naoki_Naotyu-SpeedWorld.ogg";
	track_02 = "Assets/Music/Initial_D-Deja_Vu.ogg";

	App->audio->PlayMusic(track_01.GetString());

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

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


	/* RENDER MAIN PLANE */
	plane.Render();

	//-----
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
