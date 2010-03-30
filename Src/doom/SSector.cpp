
#include <list>
#include "SSector.h"
#include "Seg.h"
#include "Node.h"
#include "Vertex.h"

namespace doom
{
	SSector::SSector(short _id)
	{
		id = _id;
		m_parentNode = NULL;
	}

	void SSector::BuildMissingSegs()
	{
		if (id==35)
			id=35;
		// Removing redundant aligned segments
		// Setting as Open segments with no other segment at one or both ends
		for (std::list<Seg*>::iterator it=m_segs.begin() ; it!=m_segs.end() ; ++it)
		{
			Seg *seg = *it;
			seg->m_backOpen = seg->m_frontOpen = true;
		}
		for (std::list<Seg*>::iterator it=m_segs.begin() ; it!=m_segs.end() ; ++it)
		{
			Seg *seg = *it;
			if (seg->m_valid == false)
				continue;
			for (std::list<Seg*>::iterator it2=m_segs.begin() ; it2!=m_segs.end() ; ++it2)
			{
				Seg *seg2 = *it2;
				if (seg == seg2)
					continue;
				if (seg2->m_valid == false)
					continue;
				if (seg->m_v2 == seg2->m_v1) {
					if (seg->Parallel(seg2)) {
						seg->m_v2 = seg2->m_v2;
						seg->m_frontOpen = seg2->m_frontOpen;
						seg2->m_valid = false;
					} else {
						seg->m_frontOpen = false;
						seg2->m_backOpen = false;
					}
				}
				if (seg2->m_valid && seg->m_v1 == seg2->m_v2) {
					if (seg->Parallel(seg2)) {
						seg->m_v1 = seg2->m_v1;
						seg->m_backOpen = seg2->m_backOpen;
						seg2->m_valid = false;
					} else {
						seg->m_backOpen = false;
						seg2->m_frontOpen = false;
					}
				}
				
				if (seg2->m_valid && seg->Aligned(seg2)) {
					if (seg->PointIsFurther(seg2->m_v2->m_x, seg2->m_v2->m_z)) {
						seg->m_v2 = seg2->m_v2;
						seg->m_frontOpen = seg2->m_frontOpen;
						seg2->m_valid = false;
					}
					if (seg->PointIsBefore(seg2->m_v1->m_x, seg2->m_v1->m_z)) {
						seg->m_v1 = seg2->m_v1;
						seg->m_backOpen = seg2->m_backOpen;
						seg2->m_valid = false;
					}
				}
			}
		}

		// Cutting open segs between themselves
		for (std::list<Seg*>::iterator it=m_segs.begin() ; it!=m_segs.end() ; ++it)
		{
			Seg *seg = *it;
			if (seg->m_valid == false)
				continue;
			if (seg->m_backOpen==false && seg->m_frontOpen==false)
				continue;
			for (std::list<Seg*>::iterator it2=m_segs.begin() ; it2!=m_segs.end() ; ++it2)
			{
				Seg *seg2 = *it2;
				if (seg == seg2)
					continue;
				if (seg2->m_valid == false)
					continue;
				if (seg2->m_backOpen==false && seg2->m_frontOpen==false)
					continue;
				seg->CutEachOther(seg2);
			}
		}
		
		// Cut open segs with bsp dividers
		Node *oldNode = NULL;
		for (Node *node = m_parentNode ; node != NULL ; node=node->m_parent)
		{
			Seg *newSeg;
			if (     (oldNode == NULL && node->m_isRightSSector && node->m_rightSSector == this)
				   ||(oldNode != NULL && (!node->m_isRightSSector) && node->m_rightNode == oldNode) ) {
				newSeg = new Seg(node->m_startVertex, node->m_endVertex);
			} else {
				newSeg = new Seg(node->m_endVertex, node->m_startVertex);
			}
			newSeg->m_backOpen = true;
			newSeg->m_frontOpen = true;
			for (std::list<Seg*>::iterator it=m_segs.begin() ; it!=m_segs.end() ; ++it)
			{
				Seg *seg = *it;
				if (seg->m_valid == false)
					continue;
				if (seg->Parallel(newSeg)) {
					if (      seg->IsOnRight(newSeg->m_v1->m_x, newSeg->m_v1->m_z) == false
						   || seg->Aligned(newSeg) ) {
						newSeg->m_valid = false;
						break;
					} else if (newSeg->IsOnRight(seg->m_v1->m_x, seg->m_v1->m_z) == false) {
						seg->m_valid = false;
						continue;
					}
				}
				seg->CutEachOther(newSeg);
				if (newSeg->m_valid == false)
					break;
			}
			if (newSeg->m_valid) //&& newSeg->m_backOpen==false && newSeg->m_frontOpen==false								)
				m_segs.push_back(newSeg); // Intersection with bsp node divider
			else
				delete newSeg;
			oldNode = node;
		}
	}
};
