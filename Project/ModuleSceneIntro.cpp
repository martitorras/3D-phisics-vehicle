#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	test_cube_physbody = nullptr;

	current_music_track = 1;


	/* MAP OBJECTS */
	plane.normal = vec3(0.0f, 1.0f, 0.0f);
	plane.constant = 0.0f;


	test_cube.size = vec3(2.0f, 2.0f, 2.0f);
	test_cube.SetPos(1.0f, 1.0f, 1.0f);
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	/* ADD BODIES */
	test_cube_physbody = App->physics->AddBody(test_cube, 10000.0f);
	test_cube_physbody->collision_listeners.add(this);

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


	/* RENDER */
	plane.Render();

	test_cube_physbody->GetTransform(&test_cube.transform);
	test_cube.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}


int ModuleSceneIntro::GetCurrentMusicTrack() const
{
	return current_music_track;
}