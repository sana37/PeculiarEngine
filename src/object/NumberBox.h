#ifndef NUMBERBOX_H
#define NUMBERBOX_H

#include "Object.h"

class NumberBox : public Object
{
public:
	explicit NumberBox(short);
//	NumberBox(const NumberBox&);
//	NumberBox(const NumberBox&, short);

	void setup(short);
	short getNum(void) const;
	static bool decompose(NumberBox**, Object**);
	char whichClass(void);

private:
	short number;
};

#endif
