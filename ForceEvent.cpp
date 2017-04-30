#include "ForceEvent.h"
#include "Force.h"
#include <iostream>//

Field::ForceEvent::ForceEvent(void) : Field::Event::Event()
{
}

void Field::ForceEvent::exec(void)
{
	for (short i = 0  ;  i < field->forceNum  ;  i++) {
		field->force[i]->exec();

		if (field->force[i]->isPermanent() == false) {
			field->finishForce(i);
		}
	}
}
