#include "SightMoveEvent.h"
#include "Sight.h"

Field::SightMoveEvent::SightMoveEvent(void) : Field::Event::Event()
{
}

void Field::SightMoveEvent::exec(void)
{
	field->sight->update();
}
