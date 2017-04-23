#include "ForceEvent.h"
#include "Force.h"

Field::ForceEvent::ForceEvent(void) : Field::Event::Event()
{
}

void Field::ForceEvent::exec(void)
{
	for (short i = 0  ;  i < field->forceNum  ;  i++) {
		field->force[i]->exec();
		field->finishForce(i);
	}
}
