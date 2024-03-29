
#ifndef SWE_CAMERA
#define SWE_CAMERA

#include "Vec3f.h"
#include "Matrix4x4.h"

class Camera
{
public:
	Vec3f m_center;
	Vec3f m_direction;
	Vec3f m_roll;
	float m_xfov, m_yfov;
	Matrix4x4 m_viewmatrix;
	Matrix4x4 m_projectionmatrix;

	Camera();
	Camera(float aspectratio);
	void RotateX(float a);
	void RotateY(float a);
	void RotateZ(float a);
	void Translate(float x, float y , float z);
};

#endif // SWE_CAMERA