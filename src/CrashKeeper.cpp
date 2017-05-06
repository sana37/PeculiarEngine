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

CrashKeeper* CrashKeeper::crashKeeper = NULL;