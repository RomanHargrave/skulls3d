
#include <math.h>
#include "Camera.h"

Camera::Camera()
{
	this->m_center.x;      this->m_center.y=0;    this->m_center.z=-1;
	this->m_direction.x=0; this->m_direction.y=0; this->m_direction.z=1;
	this->m_roll.x=0;      this->m_roll.y=1;      this->m_roll.z=0;
	//this->setXFov(90);
	//this->setYFov(60);

	float n = 1.0f;
	float f = 10.0f;
	float w = 1.0f;
	float h = 1.0f;

	this->m_projectionmatrix.m_data[0] = 2*n / w;
	this->m_projectionmatrix.m_data[5] = 2*n / h;
	this->m_projectionmatrix.m_data[10] = f / (f-n);
	this->m_projectionmatrix.m_data[11] = -f*n / (f-n);
	this->m_projectionmatrix.m_data[14] = 1;

	this->m_viewmatrix.SetIdentity();
}

Camera::Camera(float aspectratio)
{
	this->m_center.x;      this->m_center.y=0;    this->m_center.z=-1;
	this->m_direction.x=0; this->m_direction.y=0; this->m_direction.z=1;
	this->m_roll.x=0;      this->m_roll.y=1;      this->m_roll.z=0;
	//this->setXFov(90);
	//this->setYFov(60);

	float n = 1.0f;
	float f = 10.0f;
	float w = 1.0f;
	float h = 1.0f;

	if (aspectratio > 1)
	{
		this->m_projectionmatrix.m_data[0] = 2*n / w;
		this->m_projectionmatrix.m_data[5] = aspectratio * 2*n / h;
	}
	else
	{
		this->m_projectionmatrix.m_data[0] = (1.0f/aspectratio) * 2*n / w;
		this->m_projectionmatrix.m_data[5] = 2*n / h;
	}
	this->m_projectionmatrix.m_data[10] = f / (f-n);
//	this->m_projectionmatrix.m_data[11] = -f*n / (f-n);
	this->m_projectionmatrix.m_data[14] = 1;

	this->m_viewmatrix.SetIdentity();
}

void Camera::RotateX(float a)
{
	this->m_viewmatrix.RotateX(-a);
}
void Camera::RotateY(float a)
{
	this->m_viewmatrix.RotateY(-a);
}
void Camera::RotateZ(float a)
{
	this->m_viewmatrix.RotateZ(-a);
}

void Camera::Translate(float x, float y, float z)
{
	this->m_viewmatrix.Translate(-x,-y,-z);
}