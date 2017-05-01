#include "ForceEvent.h"
#include "Force.h"
#include <iostream>//

Field::ForceEvent::ForceEvent(void) : Field::Event::Event()
{
}

void Field::ForceEvent::exec(void)
{
	for (short i = 0  ;  i < field->force.length()  ;  i++) {
//		if (field->force[i]->isSetted()) {
			field->force[i]->exec();

			if (field->force[i]->isPermanent() == false) {
				field->finishForce(i);
			}
//		}
	}
/*	for (short i = 0  ;  i < field->force.length()  ;  i++) {
		if (field->force[i]->isSetted() == false) {
			
		}
	}*/
}
