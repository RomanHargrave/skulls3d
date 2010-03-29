
#include <list>
#include "SSector.h"
#include "Seg.h"
#include "Node.h"
#include "Vertex.h"

namespace doom
{
	class Segment
	{
	public:
		Vertex m_v1, m_v2;
		bool m_frontOpen, m_backOpen; // true if infinite line
		bool m_valid;

		Segment(Vertex *v1, Vertex *v2)
			:m_v1(v1->m_x, v1->m_z), m_v2(v2->m_x, v2->m_z)
		{
			m_valid = true;
		}
		bool Cut(Segment *other)
		{
			// see http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
			short denominator = (other->m_v2.m_z-other->m_v1.m_z)*(m_v2.m_x-m_v1.m_x)
			                  - (other->m_v2.m_x-other->m_v1.m_x)*(m_v2.m_z-m_v1.m_x);
			if (denominator == 0)
				return false; // Segments are parallel

			short aNumerator = (other->m_v2.m_x-other->m_v1.m_x)*(m_v1.m_z-other->m_v1.m_z)
			                 - (other->m_v2.m_z-other->m_v1.m_z)*(m_v1.m_x-other->m_v1.m_x);
			short bNumerator = (m_v2.m_x-m_v1.m_x)*(m_v1.m_z-other->m_v1.m_z)
			                 - (m_v2.m_z-m_v1.m_z)*(m_v1.m_x-other->m_v1.m_x);

			float ua = aNumerator / (float)denominator;
			float ub = bNumerator / (float)denominator;

			float otherRightX = (float)  other->m_v2.m_z - other->m_v1.m_z;
			float otherRightZ = (float)-(other->m_v2.m_x - other->m_v1.m_x);
			float thisDirX = (float)m_v2.m_x - m_v1.m_x;
			float thisDirZ = (float)m_v2.m_z - m_v1.m_z;
			float dotProduct = (float)otherRightX*thisDirX + otherRightZ*thisDirZ;
			float otherDirX = (float)other->m_v2.m_x - other->m_v1.m_x;
			float otherDirZ = (float)other->m_v2.m_z - other->m_v1.m_z;
	
			if (dotProduct >= 0)
			{
				// other's right side aligned with this segment
				if (ua >= 1)
					this->m_valid = false;
				else if (ua<1 && m_backOpen)
				{
					m_v1.m_x += (short) (ua*thisDirX);
					m_v1.m_z += (short) (ua*thisDirZ);
					m_backOpen = false;
				}
				else if (ua > 0)
				{
					m_v1.m_x += (short) (ua*thisDirX);
					m_v1.m_z += (short) (ua*thisDirZ);
				}

				if (ub <= 0)
					other->m_valid = false;
				else if (ub>0 && other->m_frontOpen)
				{
					other->m_v2.m_x = (short) (other->m_v1.m_x + ub*otherDirX);
					other->m_v2.m_z = (short) (other->m_v1.m_z + ub*otherDirZ);
					other->m_frontOpen = false;
				}
				else if (ub < 1)
				{
					other->m_v2.m_x = (short) (other->m_v1.m_x + ub*otherDirX);
					other->m_v2.m_z = (short) (other->m_v1.m_z + ub*otherDirZ);
				}
			}
			else
			{
				if (ua <= 0)
					m_valid = false;
				else if (ua>0 && m_frontOpen)
				{
					m_v2.m_x = (short) (m_v1.m_x + ua*thisDirX);
					m_v2.m_z = (short) (m_v1.m_z + ua*thisDirZ);
					m_frontOpen = false;
				}
				else if (ua < 1)
				{
					m_v2.m_x = (short) (m_v1.m_x + ua*thisDirX);
					m_v2.m_z = (short) (m_v1.m_z + ua*thisDirZ);
				}

				if (ub >= 1)
					other->m_valid = false;
				else if (ub<1 && other->m_backOpen)
				{
					other->m_v1.m_x += (short) (ub*otherDirX);
					other->m_v1.m_z += (short) (ub*otherDirZ);
					other->m_backOpen = false;
				}
				else if (ub > 0)
				{
					other->m_v1.m_x += (short) (ub*otherDirX);
					other->m_v1.m_z += (short) (ub*otherDirZ);
				}
			}
			return true;
		}
	};

	SSector::SSector(short _id)
	{
		id = _id;
	}

	void SSector::BuildMissingSegs()
	{
		std::list<Segment*> knownSegments;
		std::list<Segment*> newSegments;

		// Building Segments from known segs
		for (std::list<Seg*>::iterator it=m_segs.begin() ; it!=m_segs.end() ; ++it)
		{
			Seg *seg = *it;
			Segment *segment = new Segment(seg->v1, seg->v2);
			segment->m_frontOpen = true;
			segment->m_backOpen = true;
			for (std::list<Seg*>::iterator it2=m_segs.begin() ; it2!=m_segs.end() ; ++it2)
			{
				Seg *seg2 = *it2;
				if (seg == seg2)
					continue;
				if (seg->v2 == seg2->v1)
					segment->m_frontOpen = false;
				if (seg->v1 == seg2->v2)
					segment->m_backOpen = false;
			}
			/*if (segment->m_backOpen==false && segment->m_frontOpen==false)
			{
				delete segment;
				continue;
			}*/
			knownSegments.push_back(segment);
		}

		// cutting down segments with bsp dividers
		for (Node *node = m_parentNode ; node != NULL ; node=node->m_parent)
		{
			Segment *newsegment;
			if (node->m_rightSSector == this)
				newsegment = new Segment(node->m_startVertex, node->m_endVertex);
			else
				newsegment = new Segment(node->m_endVertex, node->m_startVertex);
			newsegment->m_backOpen = true;
			newsegment->m_frontOpen = true;
			for (std::list<Segment*>::iterator it=knownSegments.begin() ; it!=knownSegments.end() ; ++it)
			{
				Segment *segment = *it;
				if (!segment->Cut(newsegment))
					newsegment->m_valid = false;
			}
			if (newsegment->m_backOpen==false && newsegment->m_frontOpen==false && newsegment->m_valid)
				// Intersection between bsp node divider
				newSegments.push_back(newsegment);
			else
				delete newsegment;
		}

		for (std::list<Segment*>::iterator it=newSegments.begin() ; it!=newSegments.end() ; ++it)
		{
			Segment *segm = *it;
			if (segm->m_valid)
			{
				m_segs.push_back(new Seg(new Vertex(segm->m_v1.m_x, segm->m_v1.m_z),
					new Vertex(segm->m_v2.m_x, segm->m_v2.m_z)));
			}
		}
	}
};
