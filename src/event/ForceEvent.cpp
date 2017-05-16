#include "ForceEvent.h"
#include "Force.h"
#include "Impulse.h"
#include <iostream>//

Field::ForceEvent::ForceEvent(void) : Field::Event::Event()
{
}

void Field::ForceEvent::exec(void)
{
	for (short i = 0  ;  i < field->force.length()  ;  i++) {
//		if (field->force[i]->isSetted()) {
			field->force[i]->exec();

			if (field->force[i]->isDone() == true) {
				field->finishForce(i);
			}
/*
			if (dynamic_cast<Impulse*> (field->force[i]) != NULL) {//run time check spends a lot of time??
				field->finishForce(i);
			}
*/
//		}
	}
}
