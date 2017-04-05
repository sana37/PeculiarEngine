#include "Event.h"

Field::Event::Event(Field* _field)
{
	field = _field;
	execFlag = true;
}

void Field::Event::enable(void)
{
	execFlag = true;
}

void Field::Event::disable(void)
{
	execFlag = false;
}

bool Field::Event::isEnabled(void)
{
	return(execFlag);
}
