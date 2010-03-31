
#include <math.h>
#include "Seg.h"
#include "Vertex.h"


double round(double d)
{
  return (double)floor(d + 0.5);
}


namespace doom
{
	Seg::Seg(Vertex *vert1, Vertex *vert2)
	{
		m_v1 = vert1;
		m_v2 = vert2;
		m_valid = true;
	}

	bool Seg::Parallel(const Seg *other) const
	{
		return (  ((int)other->m_v2->m_z-(int)other->m_v1->m_z)*((int)m_v2->m_x-(int)m_v1->m_x)
		        - ((int)other->m_v2->m_x-(int)other->m_v1->m_x)*((int)m_v2->m_z-(int)m_v1->m_z) ) == 0;
	}

	bool Seg::Aligned(const Seg *other) const
	{
		int denominator = ((int)other->m_v2->m_z-(int)other->m_v1->m_z)*((int)m_v2->m_x-(int)m_v1->m_x)
		                - ((int)other->m_v2->m_x-(int)other->m_v1->m_x)*((int)m_v2->m_z-(int)m_v1->m_z);
		int aNumerator = ((int)other->m_v2->m_x-(int)other->m_v1->m_x)*((int)m_v1->m_z-(int)other->m_v1->m_z)
		               - ((int)other->m_v2->m_z-(int)other->m_v1->m_z)*((int)m_v1->m_x-(int)other->m_v1->m_x);
		int bNumerator = ((int)m_v2->m_x-(int)m_v1->m_x)*((int)m_v1->m_z-(int)other->m_v1->m_z)
		               - ((int)m_v2->m_z-(int)m_v1->m_z)*((int)m_v1->m_x-(int)other->m_v1->m_x);
		return (denominator==0 && aNumerator==0 && bNumerator==0);
	}

	bool Seg::PointIsFurther(int x, int z) const
	{
		if (m_v2->m_x - m_v1->m_x != 0)
			return ((x - m_v1->m_x) / (m_v2->m_x - m_v1->m_x)) > 1;
		else if (m_v2->m_z - m_v1->m_z != 0)
			return ((z - m_v1->m_z) / (m_v2->m_z - m_v1->m_z)) > 1;
		else
			return false;
	}

	bool Seg::PointIsBefore(int x, int z) const
	{
		if (m_v2->m_x - m_v1->m_x != 0)
			return ((x - m_v1->m_x) / (m_v2->m_x - m_v1->m_x)) < 0;
		else if (m_v2->m_z - m_v1->m_z != 0)
			return ((z - m_v1->m_z) / (m_v2->m_z - m_v1->m_z)) < 0;
		else
			return false;
	}
	
	void Seg::CutEachOther(Seg *other)
	{
		// see http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
		int denominator = ((int)other->m_v2->m_z-(int)other->m_v1->m_z)*((int)m_v2->m_x-(int)m_v1->m_x)
		                - ((int)other->m_v2->m_x-(int)other->m_v1->m_x)*((int)m_v2->m_z-(int)m_v1->m_z);
		if (denominator == 0)
			return; // Segments are parallel

		int aNumerator = ((int)other->m_v2->m_x-(int)other->m_v1->m_x)*((int)m_v1->m_z-(int)other->m_v1->m_z)
		               - ((int)other->m_v2->m_z-(int)other->m_v1->m_z)*((int)m_v1->m_x-(int)other->m_v1->m_x);
		int bNumerator = ((int)m_v2->m_x-(int)m_v1->m_x)*((int)m_v1->m_z-(int)other->m_v1->m_z)
		               - ((int)m_v2->m_z-(int)m_v1->m_z)*((int)m_v1->m_x-(int)other->m_v1->m_x);

		//if (aNumerator==0 && bNumerator==0)
		//	return false; // Segments are aligned

		double ua = aNumerator / (double)denominator;
		double ub = bNumerator / (double)denominator;

		int otherRightX =   (int)other->m_v2->m_z - (int)other->m_v1->m_z;
		int otherRightZ = -((int)other->m_v2->m_x - (int)other->m_v1->m_x);
		int thisDirX = (int)m_v2->m_x - (int)m_v1->m_x;
		int thisDirZ = (int)m_v2->m_z - (int)m_v1->m_z;
		int otherDirX = (int)other->m_v2->m_x - (int)other->m_v1->m_x;
		int otherDirZ = (int)other->m_v2->m_z - (int)other->m_v1->m_z;
		int dotProduct = (int)otherRightX*(int)thisDirX + (int)otherRightZ*(int)thisDirZ;

		Vertex * intersect = new Vertex(this, m_v1->m_x+(int)round(ua*thisDirX), m_v1->m_z+(int)round(ua*thisDirZ));

		if (dotProduct >= 0)
		{
			// other's right side aligned with this segment
			if (ua >= 1) {
				if (m_frontOpen == false) {
					m_valid = false;
				} else {
					m_v1 = intersect;
					m_v2 = new Vertex(this, m_v1->m_x+thisDirX, m_v1->m_z+thisDirZ);
					m_backOpen = false;
				}
			} else if (ua<1 && m_backOpen) {
				m_v1 = intersect;
				if (m_frontOpen)
					m_v2 = new Vertex(this, m_v1->m_x+thisDirX, m_v1->m_z+thisDirZ);
				m_backOpen = false;
			} else if (ua > 0) {
				m_v1 = intersect;
				if (m_frontOpen)
					m_v2 = new Vertex(this, m_v1->m_x+thisDirX, m_v1->m_z+thisDirZ);
			}
			
			if (ub <= 0) {
				if (other->m_backOpen == false) {
					other->m_valid = false;
				} else {
					other->m_v2 = intersect;
					other->m_v1 = new Vertex(this, other->m_v2->m_x-otherDirX, other->m_v2->m_z-otherDirZ);
					other->m_frontOpen = false;
				}
			} else if (ub>0 && other->m_frontOpen) {
				other->m_v2 = intersect;
				if (other->m_backOpen)
					other->m_v1 = new Vertex(this, other->m_v2->m_x-otherDirX, other->m_v2->m_z-otherDirZ);
				other->m_frontOpen = false;
			} else if (ub < 1) {
				other->m_v2 = intersect;
				if (other->m_backOpen)
					other->m_v1 = new Vertex(this, other->m_v2->m_x-otherDirX, other->m_v2->m_z-otherDirZ);
			}
		}
		else
		{
			if (ua <= 0) {
				if (m_backOpen == false) {
					m_valid = false;
				} else {
					m_v2 = intersect;
					m_v1 = new Vertex(this, m_v2->m_x-thisDirX, m_v2->m_z-thisDirZ);
					m_frontOpen = false;
				}
			} else if (ua>0 && m_frontOpen) {
				m_v2 = intersect;
				if (m_backOpen)
					m_v1 = new Vertex(this, m_v2->m_x-thisDirX, m_v2->m_z-thisDirZ);
				m_frontOpen = false;
			} else if (ua < 1) {
				m_v2 = intersect;
				if (m_backOpen)
					m_v1 = new Vertex(this, m_v2->m_x-thisDirX, m_v2->m_z-thisDirZ);
			}
			
			if (ub >= 1) {
				if (other->m_frontOpen == false) {
					other->m_valid = false;
				} else {
					other->m_v1 = intersect;
					other->m_v2 = new Vertex(this, other->m_v1->m_x+otherDirX, other->m_v1->m_z+otherDirZ);
					other->m_backOpen = false;
				}
			} else if (ub<1 && other->m_backOpen) {
				other->m_v1 = intersect;
				if (other->m_frontOpen)
					other->m_v2 = new Vertex(this, other->m_v1->m_x+otherDirX, other->m_v1->m_z+otherDirZ);
				other->m_backOpen = false;
			} else if (ub > 0) {
				other->m_v1 = intersect;
				if (other->m_frontOpen)
					other->m_v2 = new Vertex(this, other->m_v1->m_x+otherDirX, other->m_v1->m_z+otherDirZ);
			}
		}
		return;
	}
	
	bool Seg::IsOnRight(const int x, const int z) const
	{
		int xRightVec =   m_v2->m_z - m_v1->m_z;
		int zRightVec = -(m_v2->m_x - m_v1->m_x);
		int xPosVec = x - m_v1->m_x;
		int zPosVec = z - m_v1->m_z;
		int dotProduct = xRightVec*xPosVec + zRightVec*zPosVec;
		return (dotProduct >= 0);
	}
};
