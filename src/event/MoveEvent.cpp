#include "MoveEvent.h"
#include "Object.h"
#include "ObjectStatus.h"
#include "Player.h"
#include "Gunner.h"
#include "Define.h"

Field::MoveEvent::MoveEvent(void) : Field::Event::Event()
{
}

void Field::MoveEvent::exec(void)
{
	for (short i = 0; i < field->object.length(); i++) {
		if (field->object[i]->getStatus()->isReadyCrash())
			continue;

		field->object[i]->update();

		if (i >= 2) {
			if ((field->object[i]->getGravityCenter()).getMagnitude() > WORLD) {
//				std::cerr << "deleted\n";
				if (field->deadObjectNum < 100) {
					field->deadObjectIndex[field->deadObjectNum] = i;
					field->deadObjectNum++;
				}
			}
		}
	}

}
