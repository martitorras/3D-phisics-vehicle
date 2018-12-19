#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	current_music_track = 1;

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

	/* ADD BODIES */
	left_main_border = CreateCube(vec3(8.0f, 1.0f, 0.0f), vec3(2.0f, 2.0f, 60.0f));
	right_main_border = CreateCube(vec3(-8.0f, 1.0f, 0.0f), vec3(2.0f, 2.0f, 60.0f));
	goal_left_pilar = CreateCube(vec3(-10.0f, 5.0f, 0.0f), vec3(0.5f, 10.0f, 0.5f));
	goal_right_pilar = CreateCube(vec3(10.0f, 5.0f, 0.0f), vec3(0.5f, 10.0f, 0.5f));
	goal_top_beam = CreateCube(vec3(0.0f, 9.5f, 0.0f), vec3(19.5f, 1.0f, 0.5f));

	main_climb_start = CreateCube(vec3(0.0f, 1.0f, 32.0f), vec3(18.0f, 0.25f, 6.0f), 0.0f, White, -25.0f, { 1.0f, 0.0f, 0.0f });
	main_climb_finish = CreateCube(vec3(0.0f, 1.0f, 48.0f), vec3(18.0f, 0.25f, 6.0f), 0.0f, White, 25.0f, { 1.0f, 0.0f, 0.0f });

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

	/* RENDER PRIMITIVES */
	for (p2List_item<Cube>* cube_item = cubes.getFirst(); cube_item != nullptr; cube_item = cube_item->next)
	{
		cube_item->data.Render();
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