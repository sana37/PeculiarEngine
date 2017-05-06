#ifndef CRASHKEEPER_H
#define CRASHKEEPER_H

#include "Array.h"

class Object;

class CrashKeeper
{
private:
	CrashKeeper(void);
	CrashKeeper(const CrashKeeper&);

public:
	static CrashKeeper* getInstance(void);
	static void deleteInstance(void);

private:
	static CrashKeeper* crashKeeper;
	Array<Object*>* object;
};

#endif
