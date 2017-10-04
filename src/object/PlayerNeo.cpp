#include "PlayerNeo.h"
#include "Field.h"
#include "Calculater.h"

PlayerNeo::PlayerNeo(const char* fileName) :
	Object::Object(fileName),
	leftHand(new Object("res/hand")),
	rightHand(new Object("res/hand")),
	shoulder(new Object("res/shoulder")),
	sightPoint(0, 3, 8),
	lookAt(0, 0, -1),
	lookAtN(1, 0, 0)
{
	Field* field = Field::getInstance();
	field->addObject(leftHand);
	field->addObject(rightHand);
	field->addObject(shoulder);

	moveAbsolute(Vector(0, 1.5, 0));
	stop();

	leftHand->setGravityCenter(getGravityCenter());
	leftHand->moveAbsolute(??
}

PlayerNeo::PlayerNeo(const PlayerNeo& playerNeo) : Object::Object(playerNeo)
{
	sightPoint = playerNeo.sightPoint;
	lookAt = playerNeo.lookAt;
	lookAtN = playerNeo.lookAtN;

	rightHand = new Object(*(playerNeo.rightHand));
	leftHand = new Object(*(playerNeo.leftHand));
	shoulder = new Object(*(playerNeo.shoulder));

	Field* field = Field::getInstance();
//	field->addObject(this);
	field->addObject(leftHand);
	field->addObject(rightHand);
	field->addObject(shoulder);
}

PlayerNeo::~PlayerNeo(void)
{
	Field* field = Field::getInstance();
	field->deleteObject(leftHand);
	field->deleteObject(rithtHand);
	field->deleteObject(shoulder);
}

void PlayerNeo::update(void)
{
	Object::update();
	Calculater::rotateRad(&sightPoint, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&lookAt, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&lookAtN, Vector(0, 0, 0), omegaVector, omega);
//	leftHand->
	grap
	moveShoulder
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
