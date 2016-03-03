
//=============================================================================
// camera.cpp 
//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. demonstarte the usage of a camera
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a camera 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes and for assignments in the gaming courses at 
// the School of Compuer Science at Carleton University.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================


#include "stdlib.h"
#include "stdio.h"
#include "camera.h"


camera::camera(void): position(0.0,0.0,2.0), lookAtVector(0.0,0.0,-1.0), upVector(0.0,1.0,0.0), speed(0)
{

}

camera::~camera(void)
{
}

int camera::roll(float angleDeg)
{
	Vector3f rotVector = lookAtVector;

	updateOrientation(rotVector, angleDeg);

	return 0;
}

int camera::pitch(float angleDeg)
{
	Vector3f rotVector = normalize(cross(lookAtVector, upVector));

	updateOrientation(rotVector, angleDeg);

	return 0;
}

int camera::yaw(float angleDeg)
{
	Vector3f rotVector = Vector3f(0.0f, 1.0f, 0.0f);//upVector;

	updateOrientation(rotVector, angleDeg);

	return 0;
}

Vector3f camera::getPosition(void)
{
	return (position);
}


Vector3f camera::getLookAtPoint(void)
{
	return (position + lookAtVector);
}

Vector3f camera::getUpVector(void)
{
	return (upVector);
}


int camera::changePoitionDelta(float dx, float dy, float dz)
{
	position.x += dx;
	position.y += dy;
	position.z += dz;

	return 0;
}

int camera::changePositionDelta(Vector3f *dv)
{
	return 0;
}

int camera::changeAbsPoition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	return 0;
}

int camera::changeAbsPosition(Vector3f *v)
{
	return 0;
}

Vector3f camera::moveForward(float numUnits)
{
	// Add Code
	position = position + numUnits * lookAtVector;
	return (position);
}

void camera::strafe(float numUnits)
{
	Vector3f side = cross(lookAtVector, upVector);
	side = normalize(side);
	position = position + numUnits * side;
}

void camera::crane(float numUnits)
{
	position = position + numUnits * upVector;
}

int camera::updateOrientation(Vector3f rotVector, float angleRad)
{
	
	// Add Code

	Vector3f xaxis(0.0,0.0,0.0);

	// create rotation matrix around a vector
	Matrix4f rotationMatrix = Matrix4f::identity();
	float cosa = cos(angleRad);
	float sina = sin(angleRad);
	float oneC = 1 - cosa;
	float x = rotVector.x;
	float y = rotVector.y;
	float z = rotVector.z;

	rotationMatrix.vm[0].x = x*x*oneC + cosa;
	rotationMatrix.vm[0].y = x*y*oneC - z*sina;
	rotationMatrix.vm[0].z = x*z*oneC + y*sina;
	rotationMatrix.vm[1].x = y*x*oneC + z*sina;
	rotationMatrix.vm[1].y = y*y*oneC + cosa;
	rotationMatrix.vm[1].z = y*z*oneC - x*sina;
	rotationMatrix.vm[2].x = z*x*oneC - y*sina;
	rotationMatrix.vm[2].y = z*y*oneC + x*sina;
	rotationMatrix.vm[2].z = z*z*oneC + cosa;

	// rotate the camera (up vector and/or looAtVector)
	Vector4f look = rotationMatrix * Vector4f(lookAtVector, 0.0);
	lookAtVector = normalize(Vector3f(look.x, look.y, look.z));

	Vector4f up = rotationMatrix * Vector4f(upVector, 0.0);
	upVector = normalize(Vector3f(up.x, up.y, up.z));
	// update the  look-at and the up vectors using the x-axis vector


	// normalize the up  and loo-at vectors


	return 0;
}

Matrix4f camera::getViewMatrix()
{
	Matrix4f m1 = Matrix4f::identity();

	Vector3f up = normalize(upVector);
	Vector3f look = -normalize(lookAtVector);
	Vector3f side = normalize(cross(up, look));
	up = cross(look, side);
	
	m1.vm[0] = Vector4f(side.x, side.y, side.z, -dot(side,position));
	m1.vm[1] = Vector4f(up.x, up.y, up.z, -dot(up,position));
	m1.vm[2] = Vector4f(look.x, look.y, look.z, -dot(look,position));
	m1.vm[3] = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	return(m1);
}

void camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector)
{
	this->position = position;
	this->lookAtVector = lookAtPoint - position;
	this->upVector = upVector;
	this->upVector.normalize();
	this->lookAtVector.normalize();

}

// change the speed of the camera motion
int camera::updateSpeed(float speed)
{
	this->speed += speed;
	return 0;
}

float camera::getSpeed(void)
{
	return(speed);
}
