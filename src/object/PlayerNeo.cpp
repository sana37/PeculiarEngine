#include "PlayerNeo.h"

PlayerNeo::PlayerNeo(const char* fileName) : Object::Object(fileName)
{
}

PlayerNeo::PlayerNeo(const PlayerNeo& playerNeo) : Object::Object(playerNeo)
{
}

PlayerNeo::~PlayerNeo(void)
{
}

void PlayerNeo::update(void)
{
	Object::update();
}

char PlayerNeo::whichClass(void)
{
	return 'Q';
}
