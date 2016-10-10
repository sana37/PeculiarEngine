#ifndef NUMBERBOX_H
#define NUMBERBOX_H

#include "Object.h"

class NumberBox : public Object
{
public:
	NumberBox(short);
	NumberBox(const NumberBox&);
	void setup(short);
	short getNum(void) const;
	static bool decompose(NumberBox** , Object**);

private:
	short number;
};

#endif
