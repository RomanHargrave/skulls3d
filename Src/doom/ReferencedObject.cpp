
#include <list>
#include "ReferencedObject.h"

ReferencedObject::ReferencedObject(void *referencer)
{
	printf("ReferencedObject(%d)\n", referencer);
	m_objectRefs.resize(0);
	HoldObjectBy(referencer);
}
ReferencedObject::~ReferencedObject()
{
	printf("~ReferencedObject()\n");
	m_objectRefs.clear();
}

bool ReferencedObject::ObjectIsReferencedBy(const void *referencer) const
{
	printf("IsReferencedBy(%d)", referencer);

	for (std::list<void*>::const_iterator it=m_objectRefs.begin() ; it!=m_objectRefs.end() ; ++it) {
		if (((void*)*it) == referencer) {
			printf("=true\n");
			return true; // referencer already references the object
		}
	}

	printf("=false\n");
	return false;
}

void ReferencedObject::HoldObjectBy(void *referencer)
{
	printf("HoldBy(%d)\n", referencer);
	if (ObjectIsReferencedBy(referencer))
		return;
	printf("  Adding %d to hold refs\n", referencer);
	m_objectRefs.push_back(referencer);
}

void ReferencedObject::ReleaseObjectBy(const void *referencer)
{
	printf("ReleaseBy(%d)\n", referencer);
	ReleaseDataBy(referencer);
	for (std::list<void*>::const_iterator it=m_objectRefs.begin() ; it!=m_objectRefs.end() ; ++it)
	{
		if (((void*)*it) == referencer) {
			if (m_objectRefs.size() == 1) {
				m_objectRefs.clear();
				delete this;
			} else {
				m_objectRefs.erase(it);
			}
			break;
		}
	}
}

bool ReferencedObject::DataIsUsedBy(const void *referencer) const
{
	printf("IsUsedBy(%d)", referencer);
	for (std::list<void*>::const_iterator it=m_dataRefs.begin() ; it!=m_dataRefs.end() ; ++it) {
		if (((void*)*it) == referencer) {
			printf("=true\n");
			return true; // referencer already references the object
		}
	}
	printf("=false\n");
	return false;
}

void ReferencedObject::HoldDataBy(void *referencer)
{
	printf("LoadedBy(%d)\n", referencer);
	HoldObjectBy(referencer);
	if (DataIsUsedBy(referencer))
		return;
	printf("  Adding %d to use refs\n", referencer);
	m_dataRefs.push_back(referencer);
}
void ReferencedObject::ReleaseDataBy(const void *referencer)
{
	printf("UnLoadedBy(%d)\n", referencer);
	for (std::list<void*>::const_iterator it=m_dataRefs.begin() ; it!=m_dataRefs.end() ; ++it)
	{
		if (((void*)*it) == referencer) {
			if (m_dataRefs.size() == 1) {
				UnLoad();
				m_dataRefs.clear();
			} else {
				m_dataRefs.erase(it);
			}
			break;
		}
	}
}
