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

	MapCreation();
	
	//physBodies.add = App->physics->AddBody(*(rampList.getFirst()->data), 500.0f);


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	geometryList.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	p2List_item<Cube*>* itemCubes = geometryList.getFirst();

	p2List_item<PhysBody3D*>* itemBodies = physBodies.getFirst();

	while (itemBodies != nullptr && itemCubes != nullptr)
	{
		itemCubes->data->Render();
		itemBodies->data->GetTransform(&itemCubes->data->transform);
		itemCubes = itemCubes->next;
		itemBodies = itemBodies->next;
	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

Cube* ModuleSceneIntro::CreateCube(vec3 pos, vec3 size, Color rgb, float mass)
{
	Cube* cube = new Cube();
	cube->SetPos(pos.x,pos.y,pos.z);
	cube->size = size;
	cube->color = rgb;
	
	physBodies.add(App->physics->AddBody(*cube, mass));
	
	return cube;
}

Cube* ModuleSceneIntro::CreateRamp(vec3 pos, vec3 size, Color rgb, float angle, vec3 pivot)
{

	Cube* cube = new Cube();
	cube->SetRotation(angle, pivot);
	cube->SetPos(pos.x, pos.y, pos.z);
	cube->size = size;
	cube->color = rgb;
	
	return cube;
}

void ModuleSceneIntro::MapCreation()
{
	geometryList.add(CreateCube(vec3(0, 1.0f, -173.242f), vec3(360.0f,20.0f,1.0f), Blue, 100000));
	geometryList.add(CreateCube(vec3(-180.0f, 1.0f, 4.478f), vec3(1.0f,20.0f,355.597f), Blue, 100000));
	geometryList.add(CreateCube(vec3(0,1.0f,181.802f), vec3(360.0f,20.0f,1.0f), Blue, 100000));
	geometryList.add(CreateCube(vec3(179.432f,1.0f,4.478f), vec3(1.0f,20.0f,355.597f), Blue, 100000));
	//geometryList.add(CreateCube(vec3(1.581f, 0, 0), vec3(1, 0.5f, 1), Blue));


	//geometryList.add(CreateRamp(vec3(1, 1, 1), vec3(5, 5, 1), Red, 45, vec3(1, 0, 0)));

}

