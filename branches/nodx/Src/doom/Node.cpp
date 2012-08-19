
#include "Node.h"
#include "Wad.h"
#include "Vertex.h"
#include "SSector.h"
#include "lumps/LevelLump.h"
#include "..\File.h"

namespace skulls
{
	Node::Node(File & file, Level & level, unsigned int lumpPosition, unsigned int nodeIndex, Node *parent)
		:m_startVertex(0,0),
		m_endVertex(0,0)
	{
		m_parent = parent;
		file.MoveTo(lumpPosition + nodeIndex*28);

		m_startVertex.m_x = file.ReadInt2();
		m_startVertex.m_z = file.ReadInt2();
		m_endVertex.m_x   = file.ReadInt2();
		m_endVertex.m_z   = file.ReadInt2();
		m_endVertex.m_x   += m_startVertex.m_x;
		m_endVertex.m_z   += m_startVertex.m_z;

		m_rightBoundingBox.z0 = file.ReadInt2();
		m_rightBoundingBox.z1 = file.ReadInt2();
		m_rightBoundingBox.x0 = file.ReadInt2();
		m_rightBoundingBox.x1 = file.ReadInt2();
		m_leftBoundingBox.z0  = file.ReadInt2();
		m_leftBoundingBox.z1  = file.ReadInt2();
		m_leftBoundingBox.x0  = file.ReadInt2();
		m_leftBoundingBox.x1  = file.ReadInt2();

		unsigned short right, left;
		right = file.ReadInt2();
		left  = file.ReadInt2();

		if (right&0x8000) {
			m_isRightSSector = true;
			m_rightSSector = level.m_ssectors[right&0x7FFF].get();
			m_rightSSector->m_parentNode = this;
		} else {
			m_isRightSSector = false;
			m_rightNode = new Node(file, level, lumpPosition, right&0x7FFF, this);
		}
		if (left&0x8000) {
			m_isLeftSSector = true;
			m_leftSSector = level.m_ssectors[left&0x7FFF].get();
			m_leftSSector->m_parentNode = this;
		} else {
			m_isLeftSSector = false;
			m_leftNode = new Node(file, level, lumpPosition, left&0x7FFF, this);
		}
	}

	bool Node::IsOnRight(const float x, const float z) const
	{
		float xRightVec = (float)  m_endVertex.m_z - m_startVertex.m_z;
		float zRightVec = (float)-(m_endVertex.m_x - m_startVertex.m_x);
		float xPosVec = x - m_startVertex.m_x;
		float zPosVec = z - m_startVertex.m_z;
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
