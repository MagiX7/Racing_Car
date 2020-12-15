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

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	glDisable(GL_TEXTURE_2D);

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel->radius = info.wheels[0].radius;
		wheel->height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel->transform);

		wheel->Render();
	}

	q = &vehicle->getChassisWorldTransform().getRotation();
	
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis->transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q->getAxis(), q->getAngle());
	chassis->transform.M[12] += offset.getX();
	chassis->transform.M[13] += offset.getY();
	chassis->transform.M[14] += offset.getZ();

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cockpit->transform);
	btVector3 cp_offset(info.cockpit_offset.x, info.cockpit_offset.y, info.cockpit_offset.z);
	cp_offset = cp_offset.rotate(q->getAxis(), q->getAngle());
	cockpit->transform.M[12] += cp_offset.getX();
	cockpit->transform.M[13] += cp_offset.getY();
	cockpit->transform.M[14] += cp_offset.getZ();

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&leftSpoilerSupport->transform);
	btVector3 lss_offset(info.spoiler_left_support_offset.x, info.spoiler_left_support_offset.y, info.spoiler_left_support_offset.z);
	lss_offset = lss_offset.rotate(q->getAxis(), q->getAngle());
	leftSpoilerSupport->transform.M[12] += lss_offset.getX();
	leftSpoilerSupport->transform.M[13] += lss_offset.getY();
	leftSpoilerSupport->transform.M[14] += lss_offset.getZ();

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rightSpoilerSupport->transform);
	btVector3 rss_offset(info.spoiler_right_support_offset.x, info.spoiler_right_support_offset.y, info.spoiler_right_support_offset.z);
	rss_offset = rss_offset.rotate(q->getAxis(), q->getAngle());
	rightSpoilerSupport->transform.M[12] += rss_offset.getX();
	rightSpoilerSupport->transform.M[13] += rss_offset.getY();
	rightSpoilerSupport->transform.M[14] += rss_offset.getZ();

	vehicle->getChassisWorldTransform().getOpenGLMatrix(&spoiler->transform);
	btVector3 s_offset(info.spoiler_offset.x, info.spoiler_offset.y, info.spoiler_offset.z);
	s_offset = s_offset.rotate(q->getAxis(), q->getAngle());
	spoiler->transform.M[12] += s_offset.getX();
	spoiler->transform.M[13] += s_offset.getY();
	spoiler->transform.M[14] += s_offset.getZ();

	chassis->Render();
	cockpit->Render();
	leftSpoilerSupport->Render();
	rightSpoilerSupport->Render();
	spoiler->Render();

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
			// Monster Trucks turn all four wheels
			if(info.wheels[i].front == false)
				vehicle->setSteeringValue(-degrees, i);
			else
				vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}


void PhysVehicle3D::CreateCar()
{
	wheel = new Cylinder();
	wheel->color = Blue;

	chassis = new Cube(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	chassis->color = White;

	cockpit = new Cube(info.cockpit_size.x, info.cockpit_size.y, info.cockpit_size.z);
	cockpit->color = White;

	leftSpoilerSupport = new Cube(info.spoiler_left_support_size.x, info.spoiler_left_support_size.y, info.spoiler_left_support_size.z);
	leftSpoilerSupport->color = White;

	rightSpoilerSupport = new Cube(info.spoiler_right_support_size.x, info.spoiler_right_support_size.y, info.spoiler_right_support_size.z);
	rightSpoilerSupport->color = White;

	spoiler = new Cube(info.spoiler_size.x, info.spoiler_size.y, info.spoiler_size.z);
	spoiler->color = Red;
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

