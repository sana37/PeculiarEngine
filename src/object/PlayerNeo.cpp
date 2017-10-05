#include "PlayerNeo.h"
#include "Calculater.h"

PlayerNeo::PlayerNeo(const char* fileName) :
	Object::Object(fileName),
	leftHandDirection(1, 0, 0),
	shoulderDirection(0, 1, 0),
	leftHandVelocity(0),
	shoulderVelocity(0),
	sightPoint(0, 6, 12),
	sightPointN(-12, 6, -3),
	lookAt(0, 0, -1),
	lookAtN(1, 0, 0)
{
	Object* leftHand = new Object("res/hand");
	Object* rightHand = new Object("res/hand");
	Object* shoulder = new Object("res/shoulder");

	leftHand->moveRelative(-3, 0, 0);
	rightHand->moveRelative(3, 0, 0);

	this->leftStartIdx = Vector(getVertexNum() + 0.1, getPolygonNum() + 0.1, getLineNum() + 0.1);
	this->composeObject(leftHand);
	this->rightStartIdx = Vector(getVertexNum() + 0.1, getPolygonNum() + 0.1, getLineNum() + 0.1);
	this->composeObject(rightHand);
	this->shoulderStartIdx = Vector(getVertexNum() + 0.1, getPolygonNum() + 0.1, getLineNum() + 0.1);
	this->composeObject(shoulder);

	reloadRadius();

	moveAbsolute(Vector(0, 1.5, -2.0));
	stop();

	delete leftHand;
	delete rightHand;
	delete shoulder;
}

PlayerNeo::PlayerNeo(const PlayerNeo& playerNeo) : Object::Object(playerNeo)
{
	leftStartIdx = playerNeo.leftStartIdx;
	rightStartIdx = playerNeo.rightStartIdx;
	shoulderStartIdx = playerNeo.shoulderStartIdx;

	sightPoint = playerNeo.sightPoint;
	lookAt = playerNeo.lookAt;
	lookAtN = playerNeo.lookAtN;
}

PlayerNeo::~PlayerNeo(void)
{
}

void PlayerNeo::update(void)
{
	Object::update();

	Calculater::rotateRad(&sightPoint, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&sightPointN, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&lookAt, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&lookAtN, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&leftHandDirection, Vector(0, 0, 0), omegaVector, omega);
	Calculater::rotateRad(&shoulderDirection, Vector(0, 0, 0), omegaVector, omega);

	int leftVertexIdx = (int) leftStartIdx.getX();
	int rightVertexIdx = (int) rightStartIdx.getX();
	int shoulderVertexIdx = (int) shoulderStartIdx.getX();
	int lastIdx = this->getVertexNum();

	Vector deltaLeftVertex = leftHandDirection * leftHandVelocity;
	Vector deltaRightVertex = deltaLeftVertex * -1.0;
	Vector deltaShoulderVertex = shoulderDirection * shoulderVelocity;

	for (int i = leftVertexIdx; i < rightVertexIdx; i++) {
		Vector vertex = this->getVertex(i);
		this->setVertex(i, vertex + deltaLeftVertex + deltaShoulderVertex);
	}
	for (int i = rightVertexIdx; i < shoulderVertexIdx; i++) {
		Vector vertex = this->getVertex(i);
		this->setVertex(i, vertex + deltaRightVertex + deltaShoulderVertex);
	}
	for (int i = shoulderVertexIdx; i < lastIdx; i++) {
		Vector vertex = this->getVertex(i);
		this->setVertex(i, vertex + deltaShoulderVertex);
	}

}

char PlayerNeo::whichClass(void)
{
	return 'Q';
}

void PlayerNeo::moveShoulder(float speed)
{
	shoulderVelocity = speed;
}

void PlayerNeo::hold(float speed)
{
	leftHandVelocity = speed;
}

Vector PlayerNeo::getSightPoint(void)
{
	return sightPoint;
}

Vector PlayerNeo::getSightPointN(void)
{
	return sightPointN;
}

const Vector& PlayerNeo::getLookAt(void)
{
	return lookAt;
}

const Vector& PlayerNeo::getLookAtN(void)
{
	return lookAtN;
}

Vector PlayerNeo::getDeltaVertex(short idx)
{
	if (((int) leftStartIdx.getX()) <= idx  &&  idx < ((int) rightStartIdx.getX()))
		return Object::getDeltaVertex(idx) + (leftHandDirection * leftHandVelocity);
	else if (((int) rightStartIdx.getX()) <= idx  &&  idx < ((int) shoulderStartIdx.getX()))
		return Object::getDeltaVertex(idx) - (leftHandDirection * leftHandVelocity);
	else
		return Object::getDeltaVertex(idx);
}

Vector PlayerNeo::getDeltaPolygon(short idx)
{
	if (((int) leftStartIdx.getY()) <= idx  &&  idx < ((int) rightStartIdx.getY()))
		return Object::getDeltaPolygon(idx) + (leftHandDirection * leftHandVelocity);
	else if (((int) rightStartIdx.getY()) <= idx  &&  idx < ((int) shoulderStartIdx.getY()))
		return Object::getDeltaPolygon(idx) - (leftHandDirection * leftHandVelocity);
	else
		return Object::getDeltaPolygon(idx);
}

Vector PlayerNeo::getDeltaLine(short idx)
{
	if (((int) leftStartIdx.getZ()) <= idx  &&  idx < ((int) rightStartIdx.getZ()))
		return Object::getDeltaLine(idx) + (leftHandDirection * leftHandVelocity);
	else if (((int) rightStartIdx.getZ()) <= idx  &&  idx < ((int) shoulderStartIdx.getZ()))
		return Object::getDeltaLine(idx) - (leftHandDirection * leftHandVelocity);
	else
		return Object::getDeltaLine(idx);
}
