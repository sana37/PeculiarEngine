#include "MoveEvent.h"
#include "Sight.h"
#include "Object.h"
#include "Player.h"
#include "Gunner.h"

Field::MoveEvent::MoveEvent(void) : Field::Event::Event()
{
}

void Field::MoveEvent::exec(void)
{
	bool signal;

	for (short i = 0  ;  i < field->object.length()  ;  i++) {
		switch (field->object[i]->whichClass()) {
			case 'G' : {
				Gunner* gunner = dynamic_cast<Gunner*> (field->object[i]);

				signal = gunner->updateGunner();
				if (signal) {
					Object* bullet = gunner->fire();
					field->addObject(bullet);
				}
				break;
			}
			case 'P' : {
				Player* player = dynamic_cast<Player*> (field->object[i]);

				signal = player->updatePlayer();
				break;
			}
			case 'O' : {
				signal = field->object[i]->update();
				break;
			}
			case 'N' : {
				signal = field->object[i]->update();
				break;
			}
		}
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

	field->sight->receiveMovement();//I want to replace this function
}
