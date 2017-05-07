#include "CrashKeeper.h"

CrashKeeper::CrashKeeper(void)
{
}

CrashKeeper* CrashKeeper::getInstance(void)
{
	if (crashKeeper == NULL)
		crashKeeper = new CrashKeeper();
	return crashKeeper;
}

void CrashKeeper::deleteInstance(void)
{
	if (crashKeeper != NULL)
		delete crashKeeper;
	crashKeeper = NULL;
}

Array<Object*> CrashKeeper::getFloatingObjects(void)
{
	int size = object->length();
	Array<Object*> array(size);

	for (int i = 0  ;  i < size  ;  i++) {
		if ((*object)[i]->getStatus()->isFloat())
			array.add((*object)[i]);
		/////////////////////////////////////////////////////
	}
}

CrashKeeper* CrashKeeper::crashKeeper = NULL;
