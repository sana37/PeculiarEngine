#include "CrashKeeper.h"
#include "Object.h"
#include "ObjectStatus.h"

CrashKeeper::CrashKeeper(Array<Object*>* objects)
{
	object = objects;
}

CrashKeeper* CrashKeeper::getInstance(Array<Object*>* objects)
{
	if (crashKeeper == NULL)
		crashKeeper = new CrashKeeper(objects);
	return crashKeeper;
}

void CrashKeeper::deleteInstance(void)
{
	if (crashKeeper != NULL)
		delete crashKeeper;
	crashKeeper = NULL;
}

const Array< Pair<Object*> >& CrashKeeper::getAttachedObjectsPairs(void)
{
	short objectNum = object->length();
	static Array< Pair<Object*> > attachedPairs(ATTACH_PAIR_MAX);
	attachedPairs.removeAll();

	for (short i = 0  ;  i < objectNum  ;  i++) {
		const Array<Object*>& attachedObjects = (*object)[i]->getStatus()->getAttachedObjects();//&??? reference???
		for (short j = 0  ;  j < attachedObjects.length()  ;  j++) {
			attachedPairs.add(Pair<Object*>((*object)[i], attachedObjects[j]));//type translation test later
		}
	}
	int atchNum = attachedPairs.length();

	attachedPairs.removeDuplicated();

	if (atchNum != 0)
		std::cerr << atchNum << " : " << attachedPairs.length() << "\n";

	return attachedPairs;
}

const Array< Pair<Object*> >& CrashKeeper::getDetachedObjectsPairs(void)
{
	short objectNum = object->length();

	static Array< Pair<Object*> > detachedPairs(ATTACH_PAIR_MAX);
	detachedPairs.removeAll();

	for (short i = 0  ;  i < objectNum  ;  i++) {
		for (short j = i + 1  ;  j < objectNum  ;  j++) {
			detachedPairs.add(Pair<Object*>((*object)[i], (*object)[j]));
		}
	}

	const Array< Pair<Object*> >& attachedPairs = getAttachedObjectsPairs();

	for (short i = 0  ;  i < attachedPairs.length()  ;  i++) {
		detachedPairs.removeIfMatchAll(attachedPairs[i]);
	}

	return detachedPairs;
}

CrashKeeper* CrashKeeper::crashKeeper = NULL;
