
#include <list>
#include <stack>
#include "SSector.h"
#include "Seg.h"
#include "Node.h"
#include "Vertex.h"

namespace skulls
{
	SSector::SSector(short _id)
	{
		id = _id;
		m_parentNode = NULL;
	}

	void SSector::BuildMissingSegs()
	{
		/*
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

		// Cutting bsp tree segments between themselves
		std::stack<Node*> nodeStack;
		for (Node *node = m_parentNode ; node != NULL ; node=node->m_parent)
			nodeStack.push(node);
		std::list<Seg*> newSegs;
		while (nodeStack.size() > 0)
		{
			Node *node = nodeStack.top();
			Seg *newSeg;
			nodeStack.pop();
			if (nodeStack.size() > 0) {
				Node *nextNode = nodeStack.top();
				if (node->m_isRightSSector==false && node->m_rightNode==nextNode) {
					newSeg = new Seg(new Vertex(this, node->m_startVertex->m_x, node->m_startVertex->m_z),
					                 new Vertex(this, node->m_endVertex->m_x, node->m_endVertex->m_z));
				} else {
					newSeg = new Seg(new Vertex(this, node->m_endVertex->m_x, node->m_endVertex->m_z),
					                 new Vertex(this, node->m_startVertex->m_x, node->m_startVertex->m_z));
				}
			}
			else
			{
				if (node->m_isRightSSector && node->m_rightSSector==this) {
					newSeg = new Seg(new Vertex(this, node->m_startVertex->m_x, node->m_startVertex->m_z),
					                 new Vertex(this, node->m_endVertex->m_x, node->m_endVertex->m_z));
				} else {
					newSeg = new Seg(new Vertex(this, node->m_endVertex->m_x, node->m_endVertex->m_z),
					                 new Vertex(this, node->m_startVertex->m_x, node->m_startVertex->m_z));
				}
			}
			for (std::list<Seg*>::iterator it=newSegs.begin() ; it!=newSegs.end() ; ++it)
			{
				Seg *s = *it;
				if (s->m_valid == false)
					continue;
				s->CutEachOther(newSeg);
				if (newSeg->m_valid == false)
					break;
			}
			if (newSeg->m_valid == false)
			{
				delete newSeg;
				continue;
			}
			newSegs.push_back(newSeg);
		}

		// Cutting newly created bsp segments with known segments
		for (std::list<Seg*>::iterator itnew=newSegs.begin() ; itnew!=newSegs.end() ; ++itnew)
		{
			Seg *newSeg = *itnew;
			if (newSeg->m_valid == false)
				continue;
			for (std::list<Seg*>::iterator itknown=m_segs.begin() ; itknown!=m_segs.end() ; ++itknown)
			{
				Seg *knownSeg = *itknown;
				if (knownSeg->m_valid == false)
					continue;
				if (knownSeg->Parallel(newSeg)) {
					if (      knownSeg->IsOnRight(newSeg->m_v1->m_x, newSeg->m_v1->m_z) == false
						   || knownSeg->Aligned(newSeg) ) {
						newSeg->m_valid = false;
						break;
					} else if (newSeg->IsOnRight(knownSeg->m_v1->m_x, knownSeg->m_v1->m_z) == false) {
						knownSeg->m_valid = false;
						continue;
					}
				}
				newSeg->CutEachOther(knownSeg);
				if (newSeg->m_valid == false)
					break;
			}
			if (newSeg->m_valid)
				m_segs.push_back(newSeg);
		}
		*/

		/*
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
		*/
	}
};
