
#ifndef SK_REFOBJ
#define SK_REFOBJ

#include <list>

class ReferencedObject
{
private:
	std::list<void*> m_objectRefs;
	std::list<void*> m_dataRefs;
	virtual bool Load()=0;
	virtual void UnLoad()=0;

protected:
	/**
	 * The constructor creates the first reference by calling HoldObjectBy(referencer).
	 * Always call this constructor with a valid referencer. This means your ReferencedObject
	 * subclass' constructor must take a void *referencer as parameter.
	 * It must also take whatever is needed to load data. A file name for instance, so Load()
	 * opens it and loads the file's content.
	 */
	ReferencedObject(void *referencer);
	virtual ~ReferencedObject();

public:
	/**
	 * Returns true if referencer already Holds this object.
	 */
	bool ObjectIsReferencedBy(const void *referencer) const;

	/**
	 * Creates a reference to the object. A list of unique referencers is maintained.
	 * Calling HoldObjectBy() more than once with the same parameter has no effect.
	 * Creating a reference to this object guarantees it won't be deleted.
	 */
	void HoldObjectBy(void *referencer);

	/**
	 * Removes a reference to this object. If referencer was the sole referencer of this
	 * object, the object is deleted and its destructor is called.
	 * When a referencer Releases an object, ReleaseDataBy
	 * is also called, giving the same referencer as parameter, so that if referencer
	 * also had a hold on the object's data, that hold is removed too.
	 */
	void ReleaseObjectBy(const void *referencer);

	/**
	 * Returns true if referencer already has a hold on this object's data.
	 */
	bool DataIsUsedBy(const void *referencer) const;

	/**
	 * Creates a reference to the object's data. If there was none, Load() is called so
	 * the object loads its data. If there was already another reference to the object's
	 * data, the data is not loaded again, but the new reference is still added.
	 * If referencer already had a hold on the object's data, nothing happens.
	 */
	void HoldDataBy(void *referencer);

	/**
	 * Removes a reference to the object's data. If referencer was the sole referencer of the
	 * data, UnLoad() is called so the data is unloaded and memory is freed.
	 */
	void ReleaseDataBy(const void *referencer);
};

#endif // SK_REFOBJ
