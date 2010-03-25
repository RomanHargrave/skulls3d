
#ifndef SK_SSECTOR
#define SK_SSECTOR

#include <list>

namespace doom
{
	class Seg;
	class Node;

	class SSector
	{
	public:
		std::list<Seg*> m_segs;
		Node *m_parentNode;

		SSector();
		void BuildMissingSegs();
	};
};

#endif // SK_SSECTOR