#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"
#include "glut/glut.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

Cube PhysVehicle3D::CreateCube(vec3 size, vec3 offset, Color color)
{
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube cube(size.x, size.y, size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cube.transform);
	btVector3 offs(offset.x, offset.y, offset.z);
	offs = offs.rotate(q.getAxis(), q.getAngle());
	cube.transform.M[12] += offs.getX();
	cube.transform.M[13] += offs.getY();
	cube.transform.M[14] += offs.getZ();
	cube.color = color;

	return cube;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	glDisable(GL_TEXTURE_2D);
	
	Cylinder wheel;
	wheel.color = Black;
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[i].radius;
		wheel.height = info.wheels[i].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis = CreateCube(info.chassis_size, info.chassis_offset, White);
	Cube frontChassis = CreateCube(info.front_chassis_size, info.front_chassis_offset, White);
	Cube frontWingRightSupport = CreateCube(info.front_wing_right_support_size, info.front_wing_right_support_offset, White);
	Cube frontWingLeftSupport = CreateCube(info.front_wing_left_support_size, info.front_wing_left_support_offset, White);
	Cube frontWing = CreateCube(info.front_wing_size, info.front_wing_offset, Black);
	Cube frontWingRight = CreateCube(info.front_wing_right_size, info.front_wing_right_offset, Black);
	Cube frontWingLeft = CreateCube(info.front_wing_left_size, info.front_wing_left_offset, Black);
	Cube spoiler = CreateCube(info.spoiler_size, info.spoiler_offset, White);
	Cube cockpit = CreateCube(info.cockpit_size, info.cockpit_offset, White);
	Cube backCockpit = CreateCube(info.back_cockpit_size, info.back_cockpit_offset, White);
	Cube rsspoiler = CreateCube(info.right_spoiler_support_size, info.right_spoiler_support_offset, Black);
	Cube lsspoiler = CreateCube(info.left_spoiler_support_size, info.left_spoiler_support_offset, Black);

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	Line antena(info.antenna_size.x, info.antenna_size.y, info.antenna_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&antena.transform);
	btVector3 ant_offset(info.antenna_offset.x, info.antenna_offset.y, info.antenna_offset.z);
	ant_offset = ant_offset.rotate(q.getAxis(), q.getAngle());
	antena.transform.M[12] += ant_offset.getX();
	antena.transform.M[13] += ant_offset.getY();
	antena.transform.M[14] += ant_offset.getZ();
	antena.color = White;


	chassis.Render();
	frontChassis.Render();
	frontWingRightSupport.Render();
	frontWingLeftSupport.Render();
	frontWing.Render();
	frontWingRight.Render();
	frontWingLeft.Render();
	spoiler.Render();
	cockpit.Render();
	backCockpit.Render();
	antena.Render();
	rsspoiler.Render();
	lsspoiler.Render();

	glEnable(GL_TEXTURE_2D);
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			if (info.wheels[i].front == false)
				vehicle->applyEngineForce(force / 1.5, i);
			else
				vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

void PhysVehicle3D::Handbrake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (info.wheels[i].front == false)
		{
			vehicle->setBrake((btScalar)force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}


vec3 PhysVehicle3D::GetPos()
{
	vec3 position;

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);

	position.x = chassis.transform.M[12];
	position.y = chassis.transform.M[13];
	position.z = chassis.transform.M[14];

	
	return position;
}