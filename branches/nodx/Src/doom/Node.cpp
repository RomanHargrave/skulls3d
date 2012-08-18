
#include "Node.h"
#include "WadFile.h"
#include "Vertex.h"
#include "SSector.h"
#include "lumps/LevelLump.h"

namespace doom
{
	Node::Node(LevelLump *level, unsigned int lumpPosition, unsigned int nodeIndex, Node *parent)
	{
		m_parent = parent;
		m_startVertex = new Vertex(0,0);
		m_endVertex = new Vertex(0,0);
		level->m_wadfile->MoveTo(lumpPosition + nodeIndex*28);
		short dummy;
		level->m_wadfile->ReadInt2(&dummy);
		m_startVertex->m_x = (int)dummy;
		level->m_wadfile->ReadInt2(&dummy);
		m_startVertex->m_z = (int)dummy;
		level->m_wadfile->ReadInt2(&dummy);
		m_endVertex->m_x = (int) dummy;
		level->m_wadfile->ReadInt2(&dummy);
		m_endVertex->m_z = (int) dummy;
		m_endVertex->m_x += m_startVertex->m_x;
		m_endVertex->m_z += m_startVertex->m_z;

		level->m_wadfile->ReadInt2(&m_rightBoundingBox.z0);
		level->m_wadfile->ReadInt2(&m_rightBoundingBox.z1);
		level->m_wadfile->ReadInt2(&m_rightBoundingBox.x0);
		level->m_wadfile->ReadInt2(&m_rightBoundingBox.x1);
		level->m_wadfile->ReadInt2(&m_leftBoundingBox.z0);
		level->m_wadfile->ReadInt2(&m_leftBoundingBox.z1);
		level->m_wadfile->ReadInt2(&m_leftBoundingBox.x0);
		level->m_wadfile->ReadInt2(&m_leftBoundingBox.x1);

		unsigned short right, left;
		level->m_wadfile->ReadInt2((short*)&right);
		level->m_wadfile->ReadInt2((short*)&left);

		if (right&0x8000) {
			m_isRightSSector = true;
			m_rightSSector = level->m_ssectors[right&0x7FFF];
			m_rightSSector->m_parentNode = this;
		} else {
			m_isRightSSector = false;
			m_rightNode = new Node(level, lumpPosition, right&0x7FFF, this);
		}
		if (left&0x8000) {
			m_isLeftSSector = true;
			m_leftSSector = level->m_ssectors[left&0x7FFF];
			m_leftSSector->m_parentNode = this;
		} else {
			m_isLeftSSector = false;
			m_leftNode = new Node(level, lumpPosition, left&0x7FFF, this);
		}
	}

	bool Node::IsOnRight(const float x, const float z) const
	{
		float xRightVec = (float)  m_endVertex->m_z - m_startVertex->m_z;
		float zRightVec = (float)-(m_endVertex->m_x - m_startVertex->m_x);
		float xPosVec = x - m_startVertex->m_x;
		float zPosVec = z - m_startVertex->m_z;
		float dotProduct = xRightVec*xPosVec + zRightVec*zPosVec;
		return (dotProduct >= 0);
	}

	SSector* Node::GetSSByPosition(float x, float z)
	{
		if (IsOnRight(x, z))
		{
			// vectors aligned, point on the right
			if (m_isRightSSector == true)
				return m_rightSSector;
			else
				return m_rightNode->GetSSByPosition(x,z);
		}
		else
		{
			// vectors opposed, point on the left
			if (m_isLeftSSector == true)
				return m_leftSSector;
			else
				return m_leftNode->GetSSByPosition(x,z);
		}
	}

	void Node::BuildMissingSegs()
	{
		if (m_isLeftSSector)
			m_leftSSector->BuildMissingSegs();
		else 
			m_leftNode->BuildMissingSegs();

		if (m_isRightSSector)
			m_rightSSector->BuildMissingSegs();
		else
			m_rightNode->BuildMissingSegs();
	}

	int Node::Count()
	{
		if (m_isRightSSector && m_isLeftSSector)
			return 1;
		else if (m_isRightSSector && (!m_isLeftSSector))
			return 1 + m_leftNode->Count();
		else if ((!m_isRightSSector) && m_isLeftSSector)
			return m_rightNode->Count() + 1;
		else return m_rightNode->Count() + m_leftNode->Count() + 1;
	}
};
