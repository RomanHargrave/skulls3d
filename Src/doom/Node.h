
#ifndef SK_NODE
#define SK_NODE

#include "SSector.h"
#include "..\File.h"
#include "lumps/LevelLump.h"
#include "Vertex.h"

namespace skulls
{
	class Level;

	typedef struct
	{
		short x0,z0; // upper-left corner
		short x1,z1; // position, not size
	}Rect;

	class Node
	{
	public:
		Node(File & file, Level & level, unsigned int lumpPosition, unsigned int nodeIndex, Node *parent);

		bool IsOnRight(const float x, const float z) const;
		SSector * GetSSByPosition(float x, float z);
		void BuildMissingSegs();
		int Count();

	public:
		Node *m_parent;

		bool m_isLeftSSector, m_isRightSSector;
		union {
			Node *m_leftNode;
			SSector * m_leftSSector;
		};
		union {
			Node *m_rightNode;
			SSector * m_rightSSector;
		};

		Vertex m_startVertex;
		Vertex m_endVertex;
		Rect m_rightBoundingBox, m_leftBoundingBox;
	};
};

#endif // SK_NODE