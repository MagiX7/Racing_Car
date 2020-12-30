#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "glut/glut.h"
#include "Light.h"
#include "SString.h"

#define MAX_SNAKE 2
#define checkImageWidth 64
#define checkImageHeight 64
#define MAX_LIGHTS 8

struct PhysBody3D;
struct PhysMotor3D;


struct Image {

	unsigned long sizeX;

	unsigned long sizeY;

	char* data;

};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	Cube* CreateCube(vec3 pos, vec3 size, Color rgb, float mass, SString name, bool isSensor = false);
	Cube* CreateRamp(vec3 pos, vec3 size, Color rgb, float angle, vec3 pivot, SString name, float mass);

	void MapCreation(); 

	void makeCheckImage(void);

	int ImageLoad(char* filename, Image* image);

	Image* loadTexture();

	void myinit(void);

	void display(void);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2List<Cube*> geometryList;
	p2List<PhysBody3D*> physBodies;

	Cube* cube;

	GLuint texture[2];

	typedef struct Image Image;

	GLubyte checkImage[checkImageWidth][checkImageHeight][3];

	Light lights[MAX_LIGHTS];

	Torus donut;
};
