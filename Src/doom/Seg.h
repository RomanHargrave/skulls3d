
#ifndef SK_SEGS
#define SK_SEGS

namespace doom
{
	class Vertex;

	class Seg
	{
	public:
		Vertex *v1, *v2;
		bool direction;

		Seg(Vertex *vert1, Vertex *vert2, bool dir);
	};
};

#endif // SK_SEGS