
#ifndef SK_SEGS
#define SK_SEGS

namespace doom
{
	class Vertex;

	class Seg
	{
	public:
		Vertex *m_v1, *m_v2;
		bool m_frontOpen, m_backOpen; // true if infinite line
		bool m_valid;

		Seg(Vertex *vert1, Vertex *vert2);

		bool Parallel(const Seg *other) const;
		bool Aligned(const Seg *other) const;
		bool PointIsBefore(int x, int z) const;
		bool PointIsFurther(int x, int z) const;
		void CutEachOther(Seg *other);
		bool IsOnRight(const int x, const int z) const;
	};
};

#endif // SK_SEGS