#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 1, 4);
	car.chassis_offset.Set(0, 1.5, 0);

	car.front_chassis_size.Set(1.3f, 0.7f, 2.5);
	car.front_chassis_offset.Set(0, 1.5f, 3);
	
	car.front_wing_right_support_size.Set(0.2, 0.5, 0.2);
	car.front_wing_right_support_offset.Set(-0.55f, 0.9, 4);

	car.front_wing_left_support_size.Set(0.2, 0.5, 0.2);
	car.front_wing_left_support_offset.Set(0.55, 0.9, 4);

	car.front_wing_size.Set(3.2, 0.15, 0.7);
	car.front_wing_offset.Set(0, 0.575, 4.25);

	car.front_wing_right_size.Set(0.1, 0.2, 0.7);
	car.front_wing_right_offset.Set(1.55, 0.75, 4.25);

	car.front_wing_left_size.Set(0.1, 0.2, 0.7);
	car.front_wing_left_offset.Set(-1.55, 0.75, 4.25);

	car.spoiler_size.Set(2, 0.1, 0.5);
	car.spoiler_offset.Set(0, 2.5, -2.25);

	
	/*car.left_skirt_size.Set(0.2, 0.8, 2.3);
	car.left_skirt_offset.Set(0.9, 0.6, 0);
	
	car.right_skirt_size.Set(0.1, 0.8, 2.3);
	car.right_skirt_offset.Set(-0.9, 0.6, 0);
	
	car.front_bumper_size.Set(1.5, 0.8, 0.1);
	car.front_bumper_offset.Set(0, 0.65, 1.95);*/

	/*car.chassis_size.Set(3, 1.3, 5.5);
	car.chassis_offset.Set(0, 1.5, 0);
	car.cockpit_size.Set(2.5, 1, 2);
	car.cockpit_offset.Set(0, 2.6, -1);
	car.spoiler_left_support_size.Set(0.2, 0.3, 0.1);
	car.spoiler_left_support_offset.Set(-0.7, 3.2, -2);
	car.spoiler_right_support_size.Set(0.2, 0.3, 0.1);
	car.spoiler_right_support_offset.Set(0.7, 3.2, -2);
	car.spoiler_size.Set(2.5, 0.05, 0.3);
	car.spoiler_offset.Set(0, 3.3, -2.1);*/

	car.mass = 1000.0f;
	car.suspensionStiffness = 20.0f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 500.0f;
	car.frictionSlip = 100.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 2.0f;
	float wheel_radius = 0.8f;
	float wheel_width = 1.0f;
	float suspensionRestLength = 1.0f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x * 0.8f;
	float half_length = car.chassis_size.z * 0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.01f * wheel_width, connection_height, half_length - wheel_radius * -0.1f);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.01f * wheel_width, connection_height, half_length - wheel_radius * -0.1f);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.01f * wheel_width, connection_height, -half_length + wheel_radius * 0.5f);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = true;
	car.wheels[2].brake = true;
	car.wheels[2].steering = true;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.01f * wheel_width, connection_height, -half_length + wheel_radius * 0.5f);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = true;
	car.wheels[3].brake = true;
	car.wheels[3].steering = true;

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 5, 10);

	vehicle->collision_listeners.add(App->scene_intro);

	vehicle->vehicle->getRigidBody()->setUserPointer(vehicle);

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
	turn = acceleration = brake = handbrake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && vehicle->GetKmh() < 140)
	{
		acceleration = MAX_ACCELERATION;
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		handbrake = HANDBRAKE_POWER;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && vehicle->GetKmh() > -140)
	{
		acceleration = -MAX_ACCELERATION;
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Handbrake(handbrake);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}