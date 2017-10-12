#include "CrashResult.h"
#include "Object.h"
#include "PlayerNeo.h"

Field::CrashEvent::CrashResult::CrashResult(void) :
	objPlgnOrLine1(NULL),
	objVrtxOrLine2(NULL),
	result(FAIL),
	plgnIdx(-1),
	vrtxIdx(-1),
	line1Idx(-1),
	line2Idx(-1),
	dist(1.0),
	crashSpot(),
	tangencyNum(0)
//	relativeVelocity()
{
}

Field::CrashEvent::CrashResult::CrashResult(const Field::CrashEvent::CrashResult& crashResult)
{
	objPlgnOrLine1 = crashResult.objPlgnOrLine1;
	objVrtxOrLine2 = crashResult.objVrtxOrLine2;
	result = crashResult.result;
	plgnIdx = crashResult.plgnIdx;
	vrtxIdx = crashResult.vrtxIdx;
	line1Idx = crashResult.line1Idx;
	line2Idx = crashResult.line2Idx;
	dist = crashResult.dist;
	crashSpot = crashResult.crashSpot;
	tangencyNum = crashResult.tangencyNum;
//	relativeVelocity = crashResult.relativeVelocity;
}

Field::CrashEvent::CrashResult::~CrashResult(void)
{
	objPlgnOrLine1 = NULL;
	objVrtxOrLine2 = NULL;
}

const Field::CrashEvent::CrashResult& Field::CrashEvent::CrashResult::operator=(const Field::CrashEvent::CrashResult& crashResult)
{
	objPlgnOrLine1 = crashResult.objPlgnOrLine1;
	objVrtxOrLine2 = crashResult.objVrtxOrLine2;
	result = crashResult.result;
	plgnIdx = crashResult.plgnIdx;
	vrtxIdx = crashResult.vrtxIdx;
	line1Idx = crashResult.line1Idx;
	line2Idx = crashResult.line2Idx;
	dist = crashResult.dist;
	crashSpot = crashResult.crashSpot;
	tangencyNum = crashResult.tangencyNum;
//	relativeVelocity = crashResult.relativeVelocity;

	return *this;
}

void Field::CrashEvent::CrashResult::setObjPlgnAndVrtx(Object* objPlgn, Object* objVrtx, short plgnIdx, short vrtxIdx)
{
	objPlgnOrLine1 = objPlgn;
	objVrtxOrLine2 = objVrtx;
	this->plgnIdx = plgnIdx;
	this->vrtxIdx = vrtxIdx;
}

void Field::CrashEvent::CrashResult::setObjLineAndLine(Object* objLine1, Object* objLine2, short line1Idx, short line2Idx)
{
	objPlgnOrLine1 = objLine1;
	objVrtxOrLine2 = objLine2;
	this->line1Idx = line1Idx;
	this->line2Idx = line2Idx;
}

void Field::CrashEvent::CrashResult::setResult(ResultCode result)
{
	this->result = result;
}

void Field::CrashEvent::CrashResult::setDist(float dist)
{
	this->dist = dist;
}

void Field::CrashEvent::CrashResult::setCrashSpot(const Vector& vector)
{
	crashSpot = vector;
}
/*
void Field::CrashEvent::CrashResult::setRelativeVelocity(const Vector& vector)
{
	relativeVelocity = vector;
}
*/
void Field::CrashEvent::CrashResult::addTangency(void)
{
	tangencyNum++;
}

void Field::CrashEvent::CrashResult::addHandVelocityToPlayerNeo(void)
{
	Vector tempDeltaVelocity;

	if (objPlgnOrLine1->whichClass() == 'Q'  ||  objPlgnOrLine1->whichClass() == 'H') {
		PlayerNeo* playerNeo = dynamic_cast<PlayerNeo*>(objPlgnOrLine1);
		switch (result) {
		case POLYGON_AND_VERTEX:
			if (playerNeo->isLeftHand(plgnIdx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHand(plgnIdx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
//			} else if (playerNeo->isHeldObject(plgnIdx)) {
			}
			break;
		case LINE_AND_LINE:
			if (playerNeo->isLeftHandL(line1Idx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHandL(line1Idx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
			}
			break;
		}

		playerNeo->setVelocity(playerNeo->getVelocity() + tempDeltaVelocity);
	} else if (objVrtxOrLine2->whichClass() == 'Q'  ||  objVrtxOrLine2->whichClass() == 'H') {
		PlayerNeo* playerNeo = dynamic_cast<PlayerNeo*>(objVrtxOrLine2);
		switch (result) {
		case POLYGON_AND_VERTEX:
			if (playerNeo->isLeftHandV(vrtxIdx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHandV(vrtxIdx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
			}
			break;
		case LINE_AND_LINE:
			if (playerNeo->isLeftHandL(line2Idx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHandL(line2Idx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
			}
			break;
		}

		playerNeo->setVelocity(playerNeo->getVelocity() + tempDeltaVelocity);
	}
}

void Field::CrashEvent::CrashResult::restorePlayerNeo(void)
{
	Vector tempDeltaVelocity;

	if (objPlgnOrLine1->whichClass() == 'Q'  ||  objPlgnOrLine1->whichClass() == 'H') {
		PlayerNeo* playerNeo = dynamic_cast<PlayerNeo*>(objPlgnOrLine1);
		switch (result) {
		case POLYGON_AND_VERTEX:
			if (playerNeo->isLeftHand(plgnIdx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHand(plgnIdx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
			}
			break;
		case LINE_AND_LINE:
			if (playerNeo->isLeftHandL(line1Idx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHandL(line1Idx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
			}
			break;
		}

		playerNeo->setVelocity(playerNeo->getVelocity() - tempDeltaVelocity);
	} else if (objVrtxOrLine2->whichClass() == 'Q'  ||  objPlgnOrLine1->whichClass() == 'H') {
		PlayerNeo* playerNeo = dynamic_cast<PlayerNeo*>(objVrtxOrLine2);
		switch (result) {
		case POLYGON_AND_VERTEX:
			if (playerNeo->isLeftHandV(vrtxIdx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHandV(vrtxIdx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
			}
			break;
		case LINE_AND_LINE:
			if (playerNeo->isLeftHandL(line2Idx)) {
				tempDeltaVelocity = playerNeo->getLeftHandRelativeVelocity();
			} else if (playerNeo->isRightHandL(line2Idx)) {
				tempDeltaVelocity = playerNeo->getRightHandRelativeVelocity();
			}
			break;
		}

		playerNeo->setVelocity(playerNeo->getVelocity() - tempDeltaVelocity);
	}
}


Object* Field::CrashEvent::CrashResult::getObjPlgn(void)
{
	if (result == LINE_AND_LINE)
		return NULL;
	else
		return objPlgnOrLine1;
}

Object* Field::CrashEvent::CrashResult::getObjVrtx(void)
{
	if (result == LINE_AND_LINE)
		return NULL;
	else
		return objVrtxOrLine2;
}

Object* Field::CrashEvent::CrashResult::getObjLine1(void)
{
	if (result == POLYGON_AND_VERTEX)
		return NULL;
	else
		return objPlgnOrLine1;
}

Object* Field::CrashEvent::CrashResult::getObjLine2(void)
{
	if (result == POLYGON_AND_VERTEX)
		return NULL;
	else
		return objVrtxOrLine2;
}

Field::CrashEvent::CrashResult::ResultCode Field::CrashEvent::CrashResult::getResult(void)
{
	return result;
}

short Field::CrashEvent::CrashResult::getPlgnIdx(void)
{
	return plgnIdx;
}

short Field::CrashEvent::CrashResult::getVrtxIdx(void)
{
	return vrtxIdx;
}

short Field::CrashEvent::CrashResult::getLine1Idx(void)
{
	return line1Idx;
}

short Field::CrashEvent::CrashResult::getLine2Idx(void)
{
	return line2Idx;
}

float Field::CrashEvent::CrashResult::getDist(void)
{
	return dist;
}

Vector Field::CrashEvent::CrashResult::getCrashSpot(void)
{
	return crashSpot;
}

short Field::CrashEvent::CrashResult::getTangencyNum(void)
{
	return tangencyNum;
}

float Field::CrashEvent::CrashResult::getRelativeSpeed(void)
{
	return (objPlgnOrLine1->getVelocity() - objVrtxOrLine2->getVelocity()).getMagnitude()
		+ objPlgnOrLine1->getOmega().getMagnitude()
		+ objVrtxOrLine2->getOmega().getMagnitude();
}
/*
Vector Field::CrashEvent::CrashResult::getRelativeVelocity(void)
{
	return relativeVelocity;
}
*/