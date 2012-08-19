
#ifndef SK_SEGS
#define SK_SEGS

#include "Vertex.h"

namespace skulls
{
	class Vertex;

	class Seg
	{
	public:
		Seg(std::shared_ptr<Vertex> vert1, std::shared_ptr<Vertex> vert2);

		bool Parallel(const Seg *other) const;
		bool Aligned(const Seg *other) const;
		bool PointIsBefore(int x, int z) const;
		bool PointIsFurther(int x, int z) const;
		//void CutEachOther(Seg *other);
		bool IsOnRight(const int x, const int z) const;
		
	public:
		std::shared_ptr<Vertex> m_v1;
		std::shared_ptr<Vertex> m_v2;
		bool m_frontOpen, m_backOpen; // true if infinite line
		bool m_valid;
	};
};

#endif // SK_SEGS