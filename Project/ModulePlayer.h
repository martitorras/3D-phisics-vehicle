#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define TURN_DEGREES 40.0f * DEGTORAD
#define TURN_STEP_DEGREES 1.0f * DEGTORAD
#define BRAKE_POWER 35.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:
	vec3 GetPosition() const;

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;

private:
	vec3 position;

	// Manual transmission variables
	uint current_gear;
	uint max_gears;
	int* max_speeds_per_gear;
	int* max_accelerations_per_gear;

private:
	int sound_gear_shift;
};