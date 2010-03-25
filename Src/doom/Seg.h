
#ifndef SK_SEGS
#define SK_SEGS

namespace doom
{
	class Vertex;

	class Seg
	{
	public:
		Vertex *v1, *v2;

		Seg(Vertex *vert1, Vertex *vert2);
	};
};

#endif // SK_SEGS