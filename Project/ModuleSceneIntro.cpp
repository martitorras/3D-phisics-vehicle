#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	//Sphere snake
	Sphere s, s2;
	for (int i = 0; i < MAX_SNAKE; ++i)
	{
		s.radius = 10;
		s_snake[i].color = Red;
		s.SetPos(-20.0f + i*10, 20.0f, -20.0f);
		pb_snake[i] = App->physics->AddBody(s);
		if (i > 0) App->physics->AddConstraintP2P(*pb_snake[i - 1], *pb_snake[i], { 10.0f, 0, 0 }, { -10.0f, 0, 0 });
	}
	for (int i = 0; i < MAX_SNAKE; ++i)
	{
		s2.radius = 10;
		s_snake2[i].color = Blue;
		s2.SetPos(20.0f + i*10, 20.0f, 20.0f);
		pb_snake2[i] = App->physics->AddBody(s2);
		if (i > 0) App->physics->AddConstraintP2P(*pb_snake2[i - 1], *pb_snake2[i], { 10.0f, 0, 0 }, { -10.0f, 0, 0 });
	}

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

	for (int i = 0; i < MAX_SNAKE; ++i)
	{
		s_snake[i].radius = 10;
		pb_snake[i]->GetTransform(&(s_snake[i].transform));
		s_snake[i].Render();
	}

	for(int i = 0; i < MAX_SNAKE; ++i)
	{
		s_snake2[i].radius = 10;
		pb_snake2[i]->GetTransform(&(s_snake2[i].transform));
		s_snake2[i].Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

