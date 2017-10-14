#include "CrashEvent.h"
#include "Object.h"
#include "PlayerNeo.h"
#include "Force.h"
#include "Impulse.h"
#include "NumberBox.h"
#include "Calculater.h"
#include "CrashResult.h"
#include "Define.h"
#include <iostream>
#include <math.h>

Field::CrashEvent::CrashEvent(PlayerNeo* playerNeo) : Field::Event::Event()
{
	this->playerNeo = playerNeo;
}

void Field::CrashEvent::exec(void)
{
	playerNeo->initializeTouchState();

	execSecondCrash();
	execFirstCrash();

	Object* caughtObj = playerNeo->getHoldableObject();
	if (caughtObj != NULL) {
		playerNeo->composeObject(caughtObj);
		playerNeo->setHold(caughtObj->getName());
		playerNeo->stop();

		field->deleteForce(caughtObj);
		field->deleteObject(caughtObj);
	}

}

void Field::CrashEvent::execFirstCrash(void)
{
	int objectNum = field->object.length();
	Array<Object*> crashedObjects(objectNum);

	for (short i = 0; i < (objectNum - 1); i++) {
		Object* obj1 = field->object[i];

		for (short j = i + 1; j < objectNum; j++) {
			Object* obj2 = field->object[j];

			if (obj1->isFixed()  &&  obj2->isFixed())
				continue;

			if (obj1->isActive() == false  &&  obj2->isActive() == false)
				continue;

			if (canCrashObjSphere(obj1, obj2) == false)
				continue;

			short rejudgeCount = reflectIfCrash(obj1, obj2);
			if (rejudgeCount != 0) {
//				std::cerr << "rejudge:" << rejudgeCount << "\n";

				if (obj1->isFixed() == false  &&  crashedObjects.has(obj1) == false)	//should reduce calculation
					crashedObjects.add(obj1);
				if (obj2->isFixed() == false  &&  crashedObjects.has(obj2) == false)
					crashedObjects.add(obj2);
			}

		}
	}

	int count = 0;

	while (1) {
		for (short i = 0; i < crashedObjects.length(); i++) {
			Object* obj1 = crashedObjects[i];
			bool recrashed = false;

			for (short j = 0; j < objectNum; j++) {
				Object* obj2 = field->object[j];

				if (obj1 == obj2)
					continue;

				if (obj1->isActive() == false  &&  obj2->isActive() == false)
					continue;

				if (canCrashObjSphere(obj1, obj2) == false)
					continue;

				short rejudgeCount = reflectIfCrash(obj1, obj2);
				if (rejudgeCount > 0) {
	//				std::cerr << "rejudge:" << rejudgeCount << "\n";
					recrashed = true;

					if (obj2->isFixed() == false  &&  crashedObjects.has(obj2) == false)	//should reduce calculation
						crashedObjects.add(obj2);
				}
			}

			if (recrashed == false) {
				crashedObjects.remove(i);
				i--;
			}
		}

		count++;

		if (crashedObjects.length() == 0)
			break;
		else
			std::cerr << ".";

		if (count >= 10) {
			std::cerr << "\nabort recrash...\n";
			break;
		}
	}

}

void Field::CrashEvent::execSecondCrash(void)
{
}

bool Field::CrashEvent::canCrashObjSphere(Object* obj1, Object* obj2)
{
	Vector dist = obj1->getGravityCenter() - obj2->getGravityCenter();
	Vector relativeVelocity = obj1->getVelocity() - obj2->getVelocity();

	return dist.getMagnitude() <= (obj1->getRadius() + obj2->getRadius() + relativeVelocity.getMagnitude());
//	return dist.getMagnitude() <= (obj1->getRadius() + obj2->getRadius());
/*judging whether through or not in which using two judgeCrash needs relativeVelocity in this function.  otherwise doesnt need*/
}

bool Field::CrashEvent::canCrashObjSphereAndVrtx(Object* obj, Vector vrtx)
{
	Vector dist = obj->getGravityCenter() - vrtx;

	return dist.getMagnitude() <= obj->getRadius();
}

int Field::CrashEvent::reflectIfCrash(Object* obj1, Object* obj2)
{
	int count = 0;

	resolveCaught(obj1, obj2);

	while (1) {
		CrashResult result;

		judgePlgnAndVrtx(obj1, obj2, &result);
		judgePlgnAndVrtx(obj2, obj1, &result);
		judgeLineAndLine(obj1, obj2, &result);

		switch (result.getResult()) {
		case CrashResult::FAIL :
			return count;
		case CrashResult::POLYGON_AND_VERTEX :
			reflectPlgnAndVrtx(&result);
			break;
		case CrashResult::LINE_AND_LINE :
			reflectLineAndLine(&result);
			break;
		}

		count++;
		if (count >= 8) {
//			std::cerr << "cannot reflect any more\n";
			break;
		}
		if (count >= 4)
			resolveCaught(obj1, obj2);
	}

	return -1;
}

bool Field::CrashEvent::resolveCaught(Object* obj1, Object* obj2)
{
	Vector v12 = calcCaughtDist(obj1, obj2);
	Vector v21 = calcCaughtDist(obj2, obj1);
	float absV12 = v12.getMagnitude();
	float absV21 = v21.getMagnitude();

	if (absV12 == 0  &&  absV21 == 0) {
		return false;
	}

	if (absV12 > absV21) {
		if (obj1->isFixed() == false  &&  obj2->isFixed() == false) {
			obj1->moveRelative(v12 * -0.55);
			obj2->moveRelative(v12 * 0.55);
		} else if (obj2->isFixed() == false) {
			obj2->moveRelative(v12 * 1.1);
		} else if (obj1->isFixed() == false) {
			obj1->moveRelative(v12 * -1.1);
		}
	} else {
		if (obj1->isFixed() == false  &&  obj2->isFixed() == false) {
			obj1->moveRelative(v21 * 0.55);
			obj2->moveRelative(v21 * -0.55);
		} else if (obj1->isFixed() == false) {
			obj1->moveRelative(v21 * 1.1);
		} else {
			obj2->moveRelative(v21 * -1.1);
		}
	}
//	std::cerr << obj1->getName() << ", " << obj2->getName() << "\n";
	return true;
}

Vector Field::CrashEvent::calcCaughtDist(Object* objPlgn, Object* objLine)
{
	short plgnNum = objPlgn->getPolygonNum();
	short lineNum = objLine->getLineNum();

	Vector solution;
	Vector minDist;
	bool distFirstFlag = true;

	for (short j = 0; j < lineNum; j++) {
		const Vector vr = objLine->getLineRVertex(j);
		Vector lr = objLine->getLineRVertex(j) - objLine->getLineLVertex(j);
		Array<int> plgnIdList(10);
		Array<float> distList(10);

		for (short i = 0; i < plgnNum; i++) {

			if (objPlgn->isPolygonEmbody(i) == false) continue;

			const Vector p1 = objPlgn->getPolygon1Vertex(i);
			const Vector p2 = objPlgn->getPolygon2Vertex(i);
			const Vector p3 = objPlgn->getPolygon3Vertex(i);

			if (Calculater::solveCubicEquation(
				lr,
				p1 - p3,
				p2 - p3,
				vr - p3,
				&solution
			)) {
				if ((0 <= solution.getX()  &&  solution.getX() <= 1)  &&
					(0 <= solution.getY()  &&  0 <= solution.getZ())  &&
					(solution.getY() + solution.getZ() <= 1)
				) {
					plgnIdList.add(i);
					distList.add(solution.getX());
				}
			}
		}

		Vector dist;
		switch (plgnIdList.length()) {
		case 0:
			break;
		case 1:
			dist = getLineToPolygonPenetration1(objPlgn, objLine, lr, plgnIdList[0], distList[0]);
			if (minDist.getMagnitude() > dist.getMagnitude()  ||  distFirstFlag) {
				minDist = dist;
				distFirstFlag = false;
			}
			break;
		case 2:
			dist = getLineToPolygonPenetration2(objPlgn, objLine, plgnIdList[0], plgnIdList[1], j);
			if (minDist.getMagnitude() > dist.getMagnitude()  ||  distFirstFlag) {
				minDist = dist;
				distFirstFlag = false;
			}
			break;
		default:
			break;
		}

	}

	return minDist;
}

Vector Field::CrashEvent::getLineToPolygonPenetration1(Object* objPlgn, Object* objLine, const Vector& lineLR, short plgnId, float lineParam)
{
	if (objPlgn->whichClass() == 'Q') {
		if (playerNeo->isLeftHand(plgnId)) {
			playerNeo->addLeftObject(objLine);
		} else if (playerNeo->isRightHand(plgnId)) {
			playerNeo->addRightObject(objLine);
		}
	}

	Vector inside = objPlgn->getPlgnInside(plgnId);

	/**
	 * () ? inside is R : inside is L;
	 */
	Vector insideOfLine = (inside * lineLR >= 0) ? (lineLR * (-1.0 * lineParam)) : (lineLR * (1 - lineParam));
	return inside * (insideOfLine * inside);
}

Vector Field::CrashEvent::getLineToPolygonPenetration2(Object* objPlgn, Object* objLine, short plgnId1, short plgnId2, short lineId)
{
	if (objPlgn->whichClass() == 'Q') {
		if (playerNeo->isLeftHand(plgnId1)  ||  playerNeo->isLeftHand(plgnId2)) {
			playerNeo->addLeftObject(objLine);
		} else if (playerNeo->isRightHand(plgnId1)  ||  playerNeo->isRightHand(plgnId2)) {
			playerNeo->addRightObject(objLine);
		}
	}

	Array< Pair<int> > vrtxPairList = objPlgn->getCommonVertexIdxInPolygons(plgnId1, plgnId2);
	switch (vrtxPairList.length()) {
	case 2: {
		short vrtxIdx1 = vrtxPairList[0].getInstance1();
		short vrtxIdx2 = vrtxPairList[1].getInstance1();

		Vector b1, b2;
		switch (vrtxIdx1) {
		case 1: b1 = objPlgn->getPolygon1Vertex(plgnId1);	break;
		case 2: b1 = objPlgn->getPolygon2Vertex(plgnId1);	break;
		case 3: b1 = objPlgn->getPolygon3Vertex(plgnId1);	break;
		}
		switch (vrtxIdx2) {
		case 1: b2 = objPlgn->getPolygon1Vertex(plgnId1);	break;
		case 2: b2 = objPlgn->getPolygon2Vertex(plgnId1);	break;
		case 3: b2 = objPlgn->getPolygon3Vertex(plgnId1);	break;
		}
		Vector b12 = b2 - b1;

		Vector a1 = objLine->getLineRVertex(lineId);
		Vector a2 = objLine->getLineLVertex(lineId);
		Vector a12 = a2 - a1;

		Vector dist = a12 % b12;
		Vector solution;
		if (Calculater::solveCubicEquation(
			a12,
			b12,
			dist,
			b1 - a1,
			&solution
		)) {
//			std::cerr << "common polygon: execute\n";
			return dist * solution.getZ();
		}
		break;
	}
	default:
//		std::cerr << "common polygon undefined error: " << vrtxPairList.length() << "\n";
		break;
	}

	return Vector();
}

void Field::CrashEvent::judgePlgnAndVrtx(Object* objPlgn, Object* objVrtx, CrashResult* result)
{
	short plgnNum = objPlgn->getPolygonNum();
	short vrtxNum = objVrtx->getVertexNum();

	for (short j = 0; j < vrtxNum; j++) {

		if (objVrtx->isVertexEmbody(j) == false) continue;
		if (canCrashObjSphereAndVrtx(objPlgn, objVrtx->getVertex(j)) == false) continue;

		Vector deltaVertex = objVrtx->getDeltaVertex(j);

		for (short i = 0; i < plgnNum; i++) {

			if (objPlgn->isPolygonEmbody(i) == false) continue;

			Vector deltaPolygon = objPlgn->getDeltaPolygon(i);
			Vector solution;

			if (Calculater::solveCubicEquation(
				objPlgn->getPolygon2Vertex(i) - objPlgn->getPolygon1Vertex(i),
				objPlgn->getPolygon3Vertex(i) - objPlgn->getPolygon1Vertex(i),
				deltaPolygon - deltaVertex,
				objVrtx->getVertex(j) - objPlgn->getPolygon1Vertex(i),
				&solution
			)) {
				if (0 <= solution.getX()  &&  0 <= solution.getY()  &&  solution.getX() + solution.getY() <= 1) {
					if (0.0 <= solution.getZ()  &&  solution.getZ() < 1.0) {
						if (solution.getZ() < result->getDist()) {
							result->setObjPlgnAndVrtx(objPlgn, objVrtx, i, j);
							result->setDist(solution.getZ());
							result->setCrashSpot(objVrtx->getVertex(j));
							result->setResult(CrashResult::POLYGON_AND_VERTEX);

							if (objPlgn->whichClass() == 'Q') {
								if (playerNeo->isLeftHand(i)) {
									playerNeo->addLeftObject(objVrtx);
								} else if (playerNeo->isRightHand(i)) {
									playerNeo->addRightObject(objVrtx);
								}
							} else if (objVrtx->whichClass() == 'Q') {
								if (playerNeo->isLeftHandV(j)) {
									playerNeo->addLeftObject(objPlgn);
								} else if (playerNeo->isRightHandV(j)) {
									playerNeo->addRightObject(objPlgn);
								}
							}
						}
						result->addTangency();
					}
				}
			}
		}
	}

}

void Field::CrashEvent::judgeLineAndLine(Object* obj1, Object* obj2, CrashResult* result)
{
	short lineNum1 = obj1->getLineNum();
	short lineNum2 = obj2->getLineNum();

	for (short i = 0; i < lineNum1; i++) {
		Vector obj1Line = obj1->getLineLVertex(i) - obj1->getLineRVertex(i);
		Vector deltaLine1 = obj1->getDeltaLine(i);

		for (short j = 0; j < lineNum2; j++) {
			Vector solution;
			Vector deltaLine2 = obj2->getDeltaLine(j);

			if (Calculater::solveCubicEquation(
				deltaLine1 - deltaLine2,
				obj1Line,
				obj2->getLineRVertex(j) - obj2->getLineLVertex(j),
				obj2->getLineRVertex(j) - obj1->getLineRVertex(i),
				&solution
			)) {
				if ((0 <= solution.getY()  &&  solution.getY() <= 1)  &&  (0 <= solution.getZ()  &&  solution.getZ() <= 1)) {
					if (0.0 <= solution.getX()  &&  solution.getX() < 1.0) {
						if (solution.getX() < result->getDist()) {
							Vector crashSpot = (obj1->getLineRVertex(i) * (1 - solution.getY())) + (obj1->getLineLVertex(i) * solution.getY());
							result->setObjLineAndLine(obj1, obj2, i, j);
							result->setDist(solution.getX());
							result->setCrashSpot(crashSpot);
							result->setResult(CrashResult::LINE_AND_LINE);

							if (obj1->whichClass() == 'Q') {
								if (playerNeo->isLeftHandL(i)) {
									playerNeo->addLeftObject(obj2);
								} else if (playerNeo->isRightHandL(i)) {
									playerNeo->addRightObject(obj2);
								}
							} else if (obj2->whichClass() == 'Q') {
								if (playerNeo->isLeftHandL(j)) {
									playerNeo->addLeftObject(obj1);
								} else if (playerNeo->isRightHandL(j)) {
									playerNeo->addRightObject(obj1);
								}
							}
						}
						result->addTangency();
					}
				}

			}
		}
	}

}

void Field::CrashEvent::reflectPlgnAndVrtx(CrashResult* result)
{
	short plgnIdx = result->getPlgnIdx();
	Object* objPlgn = result->getObjPlgn();
	Object* objVrtx = result->getObjVrtx();

	calcRepulsion(
		objPlgn,
		objVrtx,
		objPlgn->getPolygon2Vertex(plgnIdx) - objPlgn->getPolygon1Vertex(plgnIdx),
		objPlgn->getPolygon3Vertex(plgnIdx) - objPlgn->getPolygon1Vertex(plgnIdx),
		result
	);
}

void Field::CrashEvent::reflectLineAndLine(CrashResult* result)
{
	short idx1 = result->getLine1Idx();
	short idx2 = result->getLine2Idx();
	Object* obj1 = result->getObjLine1();
	Object* obj2 = result->getObjLine2();

	calcRepulsion(
		obj1,
		obj2,
		obj1->getLineLVertex(idx1) - obj1->getLineRVertex(idx1),
		obj2->getLineLVertex(idx2) - obj2->getLineRVertex(idx2),
		result
	);
}

void Field::CrashEvent::calcRepulsion(Object* obj1, Object* obj2, const Vector& p, const Vector& q, CrashResult* result)
{
	Vector base = p % q;
	base /= base.getMagnitude();

	result->addHandVelocityToPlayerNeo();
	Impulse impulse(base, result->getCrashSpot(), obj1, obj2);
	impulse.exec();
	result->restorePlayerNeo();
}
