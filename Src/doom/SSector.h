	
#ifndef SK_SSECTOR
#define SK_SSECTOR

#include <list>

namespace skulls
{
	class Seg;
	class Node;

	class SSector
	{
	public:
		std::list<std::shared_ptr<Seg>> m_segs;
		Node *m_parentNode;
		short id;

		SSector(short id);
		void BuildMissingSegs();
	};
};

#endif // SK_SSECTOR