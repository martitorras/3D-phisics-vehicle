#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
	position = vec3(0.0f, 0.0f, 0.0f);

	current_gear = 1u;
	max_gears = 5u;

	max_speeds_per_gear = new int[max_gears] 
	{
		20, 35, 65, 90, 120
	};

	max_accelerations_per_gear = new int[max_gears]
	{						
		800, 600, 400, 325, 175
	};

	sound_gear_shift = 0;
}

ModulePlayer::~ModulePlayer()
{
	delete max_speeds_per_gear;
	delete max_accelerations_per_gear;
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	sound_gear_shift = App->audio->LoadFx("Assets/Sound_Effects/Gear_Shift.wav");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5f, 1.0f, 5.0f);
	car.chassis_offset.Set(0.0f, 1.0f, 0.0f);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = false;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = false;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(position.x, position.y, position.z);
	
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		switch (current_gear)
		{
		case 1:
			if (vehicle->GetKmh() < (float)max_speeds_per_gear[0])
			{
				acceleration = max_accelerations_per_gear[0];
			}
			break;
		case 2:
			if (vehicle->GetKmh() < (float)max_speeds_per_gear[1])
			{
				acceleration = max_accelerations_per_gear[1];
			}
			break;
		case 3:
			if (vehicle->GetKmh() < (float)max_speeds_per_gear[2])
			{
				acceleration = max_accelerations_per_gear[2];
			}
			break;
		case 4:
			if (vehicle->GetKmh() < (float)max_speeds_per_gear[3])
			{
				acceleration = max_accelerations_per_gear[3];
			}
			break;
		case 5: 
			if (vehicle->GetKmh() < (float)max_speeds_per_gear[4])
			{
				acceleration = max_accelerations_per_gear[4];
			}
			break;
		default: 
			break;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		if (current_gear < max_gears)
		{
			App->audio->PlayFx(1);
			current_gear++;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		if (current_gear > 1)
		{
			App->audio->PlayFx(1);
			current_gear--;
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_STEP_DEGREES;
	}
	else
	{
		if (turn > 0.0f)
			turn -= TURN_STEP_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_STEP_DEGREES;
	}
	else
	{
		if (turn < 0.0f)
			turn += TURN_STEP_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	// Update physbody position every frame (vehicle) ---------
	vehicle->GetPos(position.x, position.y, position.z);

	// Window title ---------
	char title[80];
	sprintf_s(title, "Speed: %.2f Km/h, Gear: %d, Turn angle: %.2f, Music: %d", vehicle->GetKmh(), current_gear, turn * RADTODEG, App->scene_intro->GetCurrentMusicTrack());
	App->window->SetTitle(title);


	return UPDATE_CONTINUE;
}

vec3 ModulePlayer::GetPosition() const
{
	return position;
}
