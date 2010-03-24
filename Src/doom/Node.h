
#ifndef SK_NODE
#define SK_NODE

namespace doom
{
	class SSector;
	class Vertex;
	class LevelLump;

	typedef struct
	{
		short x0,z0; // upper-left corner
		short x1,z1; // position, not size
	}Rect;

	class Node
	{
	public:
		Node *m_parent;

		bool m_isLeftSSector, m_isRightSSector;
		union {
			Node *m_leftNode;
			SSector *m_leftSSector;
		};
		union {
			Node *m_rightNode;
			SSector *m_rightSSector;
		};

		Vertex *m_startVertex, *m_endVertex;
		Rect m_rightBoundingBox, m_leftBoundingBox;

		Node(LevelLump *level, unsigned int lumpPosition, unsigned int nodeIndex, Node *parent);
		SSector* GetSSByPosition(float x, float z);
		int Count();
	};
};

#endif // SK_NODE