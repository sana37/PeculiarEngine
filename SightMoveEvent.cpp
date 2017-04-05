#include "SightMoveEvent.h"
#include "Sight.h"

Field::SightMoveEvent::SightMoveEvent(Field* _field) : Field::Event::Event(_field)
{
}

void Field::SightMoveEvent::exec(void)
{
	field->sight->update();
}
