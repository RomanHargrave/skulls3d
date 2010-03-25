
#include "SSector.h"
#include "Seg.h"
#include "Node.h"
#include "Vertex.h"

namespace doom
{
	class Segment;

	SSector::SSector()
	{
	}

	void SSector::BuildMissingSegs()
	{
		// Building limits from known segs

	}
	
	class Segment
	{
	public:
		Vertex v1, v2;
		bool frontOpen, backOpen; // true if infinite line

		Segment(Vertex *_v1, Vertex *_v2)
			:v1(_v1->m_x, _v1->m_z), v2(_v2->m_x, _v2->m_z)
		{
		}
	};
};
