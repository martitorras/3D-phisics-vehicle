#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	current_music_track = 1;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

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
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

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

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}


int ModuleSceneIntro::GetCurrentMusicTrack() const
{
	return current_music_track;
}