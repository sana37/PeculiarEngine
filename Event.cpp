#include "Event.h"

Field::Event::Event(void)
{
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
