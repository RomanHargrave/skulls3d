
#ifndef SK_SSECTOR
#define SK_SSECTOR

#include <list>

namespace doom
{
	class Seg;

	class SSector
	{
	public:
		std::list<doom::Seg*> m_segs;
		Node *m_parentNode;

		SSector() {};
	};
};

#endif // SK_SSECTOR