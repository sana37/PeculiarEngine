#ifndef CRASHKEEPER_H
#define CRASHKEEPER_H

#include "Array.h"
#include "Pair.h"

class Object;

class CrashKeeper
{
private:
	CrashKeeper(Array<Object*>*);
	CrashKeeper(const CrashKeeper&);

public:
	static CrashKeeper* getInstance(Array<Object*>*);
	static void deleteInstance(void);

	const Array< Pair<Object*> >& getAttachedObjectsPairs(void);
	const Array< Pair<Object*> >& getDetachedObjectsPairs(void);

private:
	static CrashKeeper* crashKeeper;
	Array<Object*>* object;
};

#endif
