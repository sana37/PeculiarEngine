#include "PlayerNeo.h"
#include "Calculater.h"

PlayerNeo::PlayerNeo(const char* fileName) : Object::Object(fileName), sightPoint(0, 4, 8), lookAt(0, 0, -1), lookAtN(1, 0, 0)
{
	moveAbsolute(Vector(0, 1.5, 0));
	stop();
}

PlayerNeo::PlayerNeo(const PlayerNeo& playerNeo) : Object::Object(playerNeo)
{
	sightPoint = playerNeo.sightPoint;
	lookAt = playerNeo.lookAt;
}

PlayerNeo::~PlayerNeo(void)
{
}

void PlayerNeo::update(void)
{
	Object::update();
	Calculater::rotateRad(&sightPoint, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&lookAt, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&lookAtN, Vector(0, 0, 0), omegaVector, omega);
}

char PlayerNeo::whichClass(void)
{
	return 'Q';
}

Vector PlayerNeo::getSightPointAbsolute(void)
{
	return this->getGravityCenter() + sightPoint;
}

const Vector& PlayerNeo::getLookAt(void)
{
	return lookAt;
}

const Vector& PlayerNeo::getLookAtN(void)
{
	return lookAtN;
}
