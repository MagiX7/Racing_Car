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
	
	Cylinder wheel;
	wheel.color = Black;
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube frontChassis(info.front_chassis_size.x, info.front_chassis_size.y, info.front_chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontChassis.transform);
	btVector3 fcOffset(info.front_chassis_offset.x, info.front_chassis_offset.y, info.front_chassis_offset.z);
	fcOffset = fcOffset.rotate(q.getAxis(), q.getAngle());
	frontChassis.transform.M[12] += fcOffset.getX();
	frontChassis.transform.M[13] += fcOffset.getY();
	frontChassis.transform.M[14] += fcOffset.getZ();

	Cube frontWingRightSupport(info.front_wing_right_support_size.x, info.front_wing_right_support_size.y, info.front_wing_right_support_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontWingRightSupport.transform);
	btVector3 fwrOffset(info.front_wing_right_support_offset.x, info.front_wing_right_support_offset.y, info.front_wing_right_support_offset.z);
	fwrOffset = fwrOffset.rotate(q.getAxis(), q.getAngle());
	frontWingRightSupport.transform[12] += fwrOffset.getX();
	frontWingRightSupport.transform[13] += fwrOffset.getY();
	frontWingRightSupport.transform[14] += fwrOffset.getZ();

	Cube frontWingLeftSupport(info.front_wing_left_support_size.x, info.front_wing_left_support_size.y, info.front_wing_left_support_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontWingLeftSupport.transform);
	btVector3 fwlOffset(info.front_wing_left_support_offset.x, info.front_wing_left_support_offset.y, info.front_wing_left_support_offset.z);
	fwlOffset = fwlOffset.rotate(q.getAxis(), q.getAngle());
	frontWingLeftSupport.transform[12] += fwlOffset.getX();
	frontWingLeftSupport.transform[13] += fwlOffset.getY();
	frontWingLeftSupport.transform[14] += fwlOffset.getZ();

	Cube frontWing(info.front_wing_size.x, info.front_wing_size.y, info.front_wing_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontWing.transform);
	btVector3 fwOffset(info.front_wing_offset.x, info.front_wing_offset.y, info.front_wing_offset.z);
	fwOffset = fwOffset.rotate(q.getAxis(), q.getAngle());
	frontWing.transform[12] += fwOffset.getX();
	frontWing.transform[13] += fwOffset.getY();
	frontWing.transform[14] += fwOffset.getZ();
	frontWing.color = Black;

	Cube frontWingRight(info.front_wing_right_size.x, info.front_wing_right_size.y, info.front_wing_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontWingRight.transform);
	btVector3 fwROffset(info.front_wing_right_offset.x, info.front_wing_right_offset.y, info.front_wing_right_offset.z);
	fwROffset = fwROffset.rotate(q.getAxis(), q.getAngle());
	frontWingRight.transform[12] += fwROffset.getX();
	frontWingRight.transform[13] += fwROffset.getY();
	frontWingRight.transform[14] += fwROffset.getZ();
	frontWingRight.color = Black;

	Cube frontWingLeft(info.front_wing_left_size.x, info.front_wing_left_size.y, info.front_wing_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&frontWingLeft.transform);
	btVector3 fwLOffset(info.front_wing_left_offset.x, info.front_wing_left_offset.y, info.front_wing_left_offset.z);
	fwLOffset = fwLOffset.rotate(q.getAxis(), q.getAngle());
	frontWingLeft.transform[12] += fwLOffset.getX();
	frontWingLeft.transform[13] += fwLOffset.getY();
	frontWingLeft.transform[14] += fwLOffset.getZ();
	frontWingLeft.color = Black;

	Cube spoiler(info.spoiler_size.x, info.spoiler_size.y, info.spoiler_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&spoiler.transform);
	btVector3 cp_offset(info.spoiler_offset.x, info.spoiler_offset.y, info.spoiler_offset.z);
	cp_offset = cp_offset.rotate(q.getAxis(), q.getAngle());
	spoiler.transform.M[12] += cp_offset.getX();
	spoiler.transform.M[13] += cp_offset.getY();
	spoiler.transform.M[14] += cp_offset.getZ();


	chassis.Render();
	frontChassis.Render();
	frontWingRightSupport.Render();
	frontWingLeftSupport.Render();
	frontWing.Render();
	frontWingRight.Render();
	frontWingLeft.Render();
	spoiler.Render();

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