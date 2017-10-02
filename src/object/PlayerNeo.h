#ifndef PLAYERNEO_H
#define PLAYERNEO_H

#include "Object.h"

class PlayerNeo : public Object
{
public:
	explicit PlayerNeo(const char*);
	PlayerNeo(const PlayerNeo&);
	virtual ~PlayerNeo(void);

	virtual void update(void);
	virtual char whichClass(void);
};

#endif
