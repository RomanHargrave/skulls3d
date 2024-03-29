
#include <memory.h>
#include <math.h>
#include "Matrix4x4.h"
#include "Vec3f.h"

Matrix4x4::Matrix4x4()
{
	memset(this->m_data, 0, sizeof(float)*16);
}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::Set(float *data)
{
	memcpy(m_data, data, sizeof(float)*16);
}

const Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const
{
	Matrix4x4 result;
	// TODO optimize

	for (unsigned int j=0 ; j<4 ; j++) {
		for (unsigned int i=0 ; i<4 ; i++) {
			for (unsigned int k=0 ; k<4 ; k++) {
				result.m_data[j*4 + k] +=
				      this->m_data[j*4 + i] * other.m_data[i*4 + k];
			}
		}
	}
	return result;
}

void Matrix4x4::Set(const Matrix4x4 &other)
{
	memcpy(this->m_data, other.m_data, sizeof(float)*16);
}

Vec3f Matrix4x4::operator*(const Vec3f & v) const
{
	Vec3f result;
	result.x = v.x*m_data[0] + v.y*m_data[1] + v.z*m_data[2] + m_data[3];
	result.y = v.x*m_data[4] + v.y*m_data[5] + v.z*m_data[6] + m_data[7];
	result.z = v.x*m_data[8] + v.y*m_data[9] + v.z*m_data[10] + m_data[11];
	return result;
}

void Matrix4x4::RotateX(float a)
{
	// fast version : 16 mul instead of 64, and no object copy
	float cosa = cos(a);
	float sina = sin(a);
	float d4 = m_data[4];
	float d5 = m_data[5];
	float d6 = m_data[6];
	float d7 = m_data[7];
	m_data[4]  =  cosa*d4 + sina*m_data[8];
	m_data[5]  =  cosa*d5 + sina*m_data[9];
	m_data[6]  =  cosa*d6 + sina*m_data[10];
	m_data[7]  =  cosa*d7 + sina*m_data[11];
	m_data[8]  = -sina*d4 + cosa*m_data[8];
	m_data[9]  = -sina*d5 + cosa*m_data[9];
	m_data[10] = -sina*d6 + cosa*m_data[10];
	m_data[11] = -sina*d7 + cosa*m_data[11];
}

void Matrix4x4::RotateY(float a)
{
	// fast version : 16 mul instead of 64, and no object copy
	float cosa = cos(a);
	float sina = sin(a);
	float d0 = m_data[0];
	float d1 = m_data[1];
	float d2 = m_data[2];
	float d3 = m_data[3];
	m_data[0]  =  cosa*d0 + sina*m_data[8];
	m_data[1]  =  cosa*d1 + sina*m_data[9];
	m_data[2]  =  cosa*d2 + sina*m_data[10];
	m_data[3]  =  cosa*d3 + sina*m_data[11];
	m_data[8]  = -sina*d0 + cosa*m_data[8];
	m_data[9]  = -sina*d1 + cosa*m_data[9];
	m_data[10] = -sina*d2 + cosa*m_data[10];
	m_data[11] = -sina*d3 + cosa*m_data[11];
}

void Matrix4x4::RotateZ(float a)
{
	// fast version : 16 mul instead of 64, and no object copy
	float cosa = cos(a);
	float sina = sin(a);
	float d0 = m_data[0];
	float d1 = m_data[1];
	float d2 = m_data[2];
	float d3 = m_data[3];
	m_data[0] =  cosa*d0 + sina*m_data[4];
	m_data[1] =  cosa*d1 + sina*m_data[5];
	m_data[2] =  cosa*d2 + sina*m_data[6];
	m_data[3] =  cosa*d3 + sina*m_data[7];
	m_data[4] = -sina*d0 + cosa*m_data[4];
	m_data[5] = -sina*d1 + cosa*m_data[5];
	m_data[6] = -sina*d2 + cosa*m_data[6];
	m_data[7] = -sina*d3 + cosa*m_data[7];
}

void Matrix4x4::SetRotateXY(float x, float y)
{
	float cosx = cos(x);
	float sinx = sin(x);
	float cosy = cos(y);
	float siny = sin(y);
	m_data[15] = 1;
	m_data[3] = m_data[4] = m_data[7] = m_data[11] = m_data[12] = m_data[13] = m_data[14] = 0;
	m_data[0] = cosy;
	m_data[1] = -sinx*siny;
	m_data[2] = siny*cosx;
	m_data[5] = cosx;
	m_data[6] = sinx;
	m_data[8] = -siny;
	m_data[9] = -sinx*cosy;
	m_data[10] = cosx*cosy;
}

void Matrix4x4::Translate(float x, float y, float z)
{
	this->m_data[3] += x;
	this->m_data[7] += y;
	this->m_data[11] += z;
}

void Matrix4x4::SetIdentity()
{
	memset(this->m_data, 0, sizeof(float)*16);
	this->m_data[0] = this->m_data[5] = this->m_data[10] = this->m_data[15] = 1;
}