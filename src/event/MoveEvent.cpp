#include "MoveEvent.h"
#include "Object.h"
//#include "ObjectStatus.h"
#include "Player.h"
#include "Gunner.h"
#include "Define.h"

Field::MoveEvent::MoveEvent(void) : Field::Event::Event()
{
}

void Field::MoveEvent::exec(void)
{
	for (short i = 0; i < field->object.length(); i++) {
//		if (field->object[i]->getStatus()->isReadyCrash())
//			continue;

		field->object[i]->update();

	}

}
