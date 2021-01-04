#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "Bullet/include/LinearMath/btVector3.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 9000.0f
#define TURN_DEGREES 12.0f * DEGTORAD
#define BRAKE_POWER 5000.0f
#define HANDBRAKE_POWER 1500.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void HandleInputs(float dt);

	btVector3 RotateVecToLocal(float x, float y, float z);

public:

	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	float handbrake;

	float turbo;
	int turbosLeft;

	btVector3 initialPosition;

	bool groundContact;
};