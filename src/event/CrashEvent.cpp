#include "CrashEvent.h"
#include "Object.h"
#include "Force.h"
#include "Impulse.h"
#include "StickForce.h"
#include "NumberBox.h"
#include "Calculater.h"
#include "CrashResult.h"
#include "CrashKeeper.h"
#include "Define.h"
#include <iostream>
#include <math.h>

Field::CrashEvent::CrashEvent(void) : Field::Event::Event()
{
	crashKeeper = CrashKeeper::getInstance(&(field->object));
}

void Field::CrashEvent::exec(void)
{
	execSecondCrash();
	execFirstCrash();
}

void Field::CrashEvent::execFirstCrash(void)
{
//	const Array< Pair<Object*> >& pairs = crashKeeper->getDetachedObjectsPairs();
	int objectNum = field->object.length();
	Array<Object*> crashedObjects(objectNum);

/*
	for (short i = 0; i < pairs.length(); i++) {
		Object* obj1 = pairs[i].getInstance1();
		Object* obj2 = pairs[i].getInstance2();
*/
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

		if (crashedObjects.length() == 0)
			break;
		else
			std::cerr << ".";
	}

}

void Field::CrashEvent::execSecondCrash(void)
{
/*
	const Array< Pair<Object*> >& pairs = crashKeeper->getAttachedObjectsPairs();

	for (short i = 0  ;  i < pairs.length()  ;  i++) {
		Object* obj1 = pairs[i].getInstance1();
		Object* obj2 = pairs[i].getInstance2();

		if (reflectIfCrash(obj1, obj2)) {
		}
	}
*/
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

	while (1) {
		CrashResult result;

		judgePlgnAndVrtx(obj1, obj2, &result);
		judgePlgnAndVrtx(obj2, obj1, &result);
		judgeLineAndLine(obj1, obj2, &result);

		switch (result.getResult()) {
		case CrashResult::FAIL :
			if (count >= 5)
				std::cerr << "success resolve!! 1\n";
			return count;
		case CrashResult::POLYGON_AND_VERTEX :
			reflectPlgnAndVrtx(&result);
			break;
		case CrashResult::LINE_AND_LINE :
			reflectLineAndLine(&result);
			break;
		default :
			if (count >= 5)
				std::cerr << "success resolve!! 2\n";
			return count;
		}

		count++;
		if (count >= 8) {
			std::cerr << "cannot reflect any more\n";
			break;
		}
		if (count >= 5) {
//			std::cerr << "let's resolve\n";
			resolveCaught(obj1, obj2, &result);
		}
	}

	return -1;
}

void Field::CrashEvent::resolveCaught(Object* obj1, Object* obj2, CrashResult* result)
{
	Vector v12 = calcCaughtDist(obj1, obj2);
	Vector v21 = calcCaughtDist(obj2, obj1);
	float absV12 = v12.getMagnitude();
	float absV21 = v21.getMagnitude();

	if (absV12 == 0  &&  absV21 == 0) {
/*		if (obj1->isFixed() == false)
			obj1->back();
		if (obj2->isFixed() == false)
			obj2->back();
*/
//		std::cerr << "zero back\n";
//		field->timeControl();//
		return;
	}

	if (absV12 > absV21) {
		if (obj2->isFixed() == false) {
			obj2->moveRelative(v12 * 1.1);
			std::cerr << "1\n";
		} else {
			obj1->moveRelative(v12 * -1.1);
			std::cerr << "2\n";
		}
	} else {
		if (obj1->isFixed() == false) {
			obj1->moveRelative(v21 * 1.1);
			std::cerr << "3\n";
		} else {
			obj2->moveRelative(v21 * -1.1);
			std::cerr << "4\n";
		}
	}
	field->timeControl();//
	std::cerr << obj1->getName() << ", " << obj2->getName() << "\n";
}

Vector Field::CrashEvent::calcCaughtDist(Object* objPlgn, Object* objLine)
{
	short plgnNum = objPlgn->getPolygonNum();
	short lineNum = objLine->getLineNum();

	Vector solution;
	Vector maxDist;

//	std::cerr << "penetrated:";

	for (short j = 0; j < lineNum; j++) {
		short count = 0;
		const Vector vr = objLine->getLineRVertex(j);
		Vector lr = objLine->getLineRVertex(j) - objLine->getLineLVertex(j);
		Array<int> plgnIdList;
		Array<int> distList;

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
					count++;
					plgnIdList.add(i);
					distList.add(solution.getX());
				}
			}
		}

		switch (count) {
		case 0:
			break;
		case 1: {
			Vector inside = objPlgn->getPlgnInside(plgnIdList[0]);

			// () ? inside is R : inside is L;
//			Vector dist = (inside * lr >= 0) ? (lr * (-1.0 * distList[0])) : (lr * (1 - distList[0]));
			Vector insideOfLine = (inside * lr >= 0) ? (lr * (-1.0 * distList[0])) : (lr * (1 - distList[0]));
			Vector dist = inside * (insideOfLine * inside);
			std::cerr << dist.getMagnitude() << ", ";

			if (maxDist.getMagnitude() < dist.getMagnitude())
				maxDist = dist;
			break;
		}
		case 2:
/*
			if (distList[0] < distList[1]) {
				(inside[0] * lr >= 0) ? lr
			}
*/
			break;
		default:
			break;
		}
//		std::cerr << count << ", ";
	}

	std::cerr << "\n";

	return maxDist;
}

void Field::CrashEvent::judgePlgnAndVrtx(Object* objPlgn, Object* objVrtx, CrashResult* result)
{
	short plgnNum = objPlgn->getPolygonNum();
	short vrtxNum = objVrtx->getVertexNum();

	for (short j = 0; j < vrtxNum; j++) {

		if (objVrtx->isVertexEmbody(j) == false) continue;
		if (canCrashObjSphereAndVrtx(objPlgn, objVrtx->getVertex(j)) == false) {
//			std::cerr << '.';
			continue;
		} else {
//			std::cerr << '_';
		}

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
	Vector omega = field->object[3]->getOmega();

	Vector base = p % q;
	base /= base.getMagnitude();

//	float relativeVelocity = result->getRelativeVelocity() * base;

	if (result->getRelativeSpeed() < ZERO_VELOCITY) {//relative vellocity should be calculated more exactly
		std::cerr << "stick\n";
		Force* stickForce;
		switch (result->getTangencyNum()) {
		case 2 :
//			std::cerr << "undefined\n";
		case 1 :
//			std::cerr << "stick\n";
			stickForce = new StickForce(base, obj1, obj2, *result);
			stickForce->exec();
			delete stickForce;
			break;
		case 0 :
//			std::cerr << "undefined more\n";
			break;
		default :
			std::cerr << "coming!!!!!!!!!!!!!!!!!!!!!!!\n";
			obj1->stop();
			obj2->stop();
			break;
		}
	} else {
//		std::cerr << "repulsion\n";
		Impulse impulse(base, result->getCrashSpot(), obj1, obj2);
		impulse.exec();
	}
//	field->timeControl();

/*		migitekei or hidaritekei ??  you must confirm it
	Vector degVelocity1 = obj1->getOmega() % (result->getCrashSpot() - obj1->getGravityCenter());
	Vector degVelocity2 = obj2->getOmega() % (result->getCrashSpot() - obj2->getGravityCenter());
*/
/*
	Vector radiusVector1 = obj1->getGravityCenter() - result->getCrashSpot();
	Vector omega1 = obj1->getOmega();
	float radius1 = radiusVector1.getMagnitude();
	Calculater::rotate(&omega1, Vector(0, 0, 0), radiusVector1 / radius1, 90.0);
	omega1 *= radius1;

	Vector radiusVector2 = obj2->getGravityCenter() - result->getCrashSpot();
	Vector omega2 = obj2->getOmega();
	float radius2 = radiusVector2.getMagnitude();
	Calculater::rotate(&omega2, Vector(0, 0, 0), radiusVector2 / radius2, 90.0);
	omega2 *= radius2;
*/

//	float radius2 = (result->getCrashSpot() - obj2->getGravityCenter()).getMagnitude();
//	Vector v = (obj1->getVelocity() + omega1) - (obj2->getVelocity() + omega2);

/*
	Vector v = obj1->getVelocity() - obj2->getVelocity();
	Vector solution;

	if (Calculater::solveCubicEquation(p, q, n, v, &solution)) {
//		Vector vector = n * solution.getZ() * (1 + e) * (m1 * m2 / (m1 + m2));
		Vector rv = n * solution.getZ();

			float m1 = obj1->getMass();
			float m2 = obj2->getMass();
			float e = 0.7;

//			vector *= (1 + e) * (m1 * m2 / (m1 + m2));
			rv *= -(1 + e) * (m1 * m2 / (m1 + m2));
			Force* impulse = new Impulse(rv, result->getCrashSpot(), obj1, obj2);
			field->addForce(impulse);
	} else {
		std::cerr << "through?\n";
	}
*/
}
