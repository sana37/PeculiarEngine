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
	const Array< Pair<Object*> >& pairs = crashKeeper->getDetachedObjectsPairs();

	for (short i = 0  ;  i < pairs.length()  ;  i++) {
		Object* obj1 = pairs[i].getInstance1();
		Object* obj2 = pairs[i].getInstance2();

		if (obj1->isActive() == false  &&  obj2->isActive() == false)
			continue;

		if (canCrashObjSphere(obj1, obj2) == false)
			continue;

		if (reflectIfCrash(obj1, obj2)) {
/*
			short result = -1;
			if (obj1->whichClass() == 'N'  &&  obj2->whichClass() == 'O'  &&  j >= 3) {
				std::cerr << j << '\n';
				NumberBox* numberBox = dynamic_cast<NumberBox*> (obj1);
				Object* object = obj2;
				result = NumberBox::decompose(&numberBox, &object);
//				obj1 = numberBox;
				field->object.set(i, numberBox);
				field->object.set(j, object);
				std::cerr << "change end\n";
				field->reportScore(result);
			}
			if (obj1->whichClass() == 'O'  &&  obj2->whichClass() == 'N'  &&  i >= 3) {
				std::cerr << i << '\n';
				NumberBox* numberBox = dynamic_cast<NumberBox*> (obj2);
				Object* object = obj1;
				result = NumberBox::decompose(&numberBox, &object);
//				obj2 = numberBox;
				field->object.set(j, numberBox);
				field->object.set(i, object);
				std::cerr << "change end\n";
				field->reportScore(result);
			}
*/
		}
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

bool Field::CrashEvent::reflectIfCrash(Object* obj1, Object* obj2)
{
	CrashResult result;

	judgePlgnAndVrtx(obj1, obj2, &result);
/*
	if (result.getResult() == true) {
		reflectPlgnAndVrtx(obj1, obj2, &result);
//		std::cerr << "A";
		return true;
	}
*/
	judgePlgnAndVrtx(obj2, obj1, &result);
/*
	if (result.getResult() == true) {
		reflectPlgnAndVrtx(obj2, obj1, &result);
//		std::cerr << "B";
		return true;
	}
*/
	judgeLineAndLine(obj1, obj2, &result);
/*
	if (result.getResult() == true) {
		reflectLineAndLine(obj1, obj2, &result);
//		std::cerr << "C";
		return true;
	}
*/
	switch (result.getResult()) {
	case CrashResult::FAIL :
		return false;
	case CrashResult::POLYGON_AND_VERTEX :
		reflectPlgnAndVrtx(&result);
		return true;
	case CrashResult::LINE_AND_LINE :
		reflectLineAndLine(&result);
		return true;
	default :
		return false;
	}

}

void Field::CrashEvent::judgePlgnAndVrtx(Object* objPlgn, Object* objVrtx, CrashResult* result)
{
	short plgnNum = objPlgn->getPolygonNum();
	short vrtxNum = objVrtx->getVertexNum();

	Vector relativeVelocity = objPlgn->getVelocity() - objVrtx->getVelocity();

	for (short j = 0  ;  j < vrtxNum  ;  j++) {

		if (objVrtx->isVertexEmbody(j) == false) continue;
		if (canCrashObjSphereAndVrtx(objPlgn, objVrtx->getVertex(j)) == false) {
//			std::cerr << '.';
			continue;
		} else {
//			std::cerr << '_';
		}

		Vector vrtxVelocity = objVrtx->getOmega() % (objVrtx->getVertex(j) - objVrtx->getGravityCenter());

		for (short i = 0  ;  i < plgnNum  ;  i++) {

			if (objPlgn->isPolygonEmbody(i) == false) continue;

			Vector relativeOmega = (objPlgn->getOmega() % (objPlgn->getPolygon1Vertex(i) - objPlgn->getGravityCenter())) - vrtxVelocity;
			Vector solution;

			if (Calculater::solveCubicEquation(
				objPlgn->getPolygon2Vertex(i) - objPlgn->getPolygon1Vertex(i),
				objPlgn->getPolygon3Vertex(i) - objPlgn->getPolygon1Vertex(i),
//				relativeVelocity,
				relativeVelocity + relativeOmega,
				objVrtx->getVertex(j) - objPlgn->getPolygon1Vertex(i),
				&solution
			)) {
				if (0 <= solution.getX()  &&  0 <= solution.getY()  &&  solution.getX() + solution.getY() <= 1) {
					if (0 <= solution.getZ()  &&  solution.getZ() < result->getDist()) {
						result->setObjPlgnAndVrtx(objPlgn, objVrtx, i, j);
						result->setDist(solution.getZ());
						result->setCrashSpot(objVrtx->getVertex(j));
						result->setRelativeVelocity(relativeVelocity + relativeOmega);
						result->setResult(CrashResult::POLYGON_AND_VERTEX);
//						return;
					}
				}
			}
		}
	}

//	result->setResult(false);
}

void Field::CrashEvent::judgeLineAndLine(Object* obj1, Object* obj2, CrashResult* result)
{
	short lineNum1 = obj1->getLineNum();
	short lineNum2 = obj2->getLineNum();

	Vector relativeVelocity = obj1->getVelocity() - obj2->getVelocity();

	for (short i = 0  ;  i < lineNum1  ;  i++) {
		Vector obj1Line = obj1->getLineLVertex(i) - obj1->getLineRVertex(i);
		Vector line1Velocity = obj1->getOmega() % (obj1->getLineLVertex(i) - obj1->getGravityCenter());//naming sense... and reduce calculation

		for (short j = 0  ;  j < lineNum2  ;  j++) {
			Vector solution;
			Vector relativeOmega = line1Velocity - (obj2->getOmega() % (obj2->getLineLVertex(j) - obj2->getGravityCenter()));

			if (Calculater::solveCubicEquation(
//				relativeVelocity,
				relativeVelocity + relativeOmega,
				obj1Line,
				obj2->getLineRVertex(j) - obj2->getLineLVertex(j),
				obj2->getLineRVertex(j) - obj1->getLineRVertex(i),
				&solution
			)) {
				if ((0 <= solution.getY()  &&  solution.getY() <= 1)  &&  (0 <= solution.getZ()  &&  solution.getZ() <= 1)) {
					if (0 <= solution.getX()  &&  solution.getX() < result->getDist()) {
						Vector crashSpot = (obj1->getLineRVertex(i) * (1 - solution.getY())) + (obj1->getLineLVertex(i) * solution.getY());
						result->setObjLineAndLine(obj1, obj2, i, j);
						result->setDist(solution.getX());
						result->setCrashSpot(crashSpot);
						result->setRelativeVelocity(relativeVelocity + relativeOmega);
						result->setResult(CrashResult::LINE_AND_LINE);
//						return;
					}
				}

			}
		}
	}

//	result->setResult(false);
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
	std::cerr << omega.getX() << " : " << omega.getY() << " : " << omega.getZ() << "\n";
	std::cerr << "idx : " << result->getVrtxIdx() << "\n";

	Vector base = p % q;
	base /= base.getMagnitude();

	float relativeVelocity = result->getRelativeVelocity() * base;

	if (-ZERO_VELOCITY < relativeVelocity  &&  relativeVelocity < ZERO_VELOCITY) {
		std::cerr << "stick\n";
		Force* stickForce = new StickForce(base, obj1, obj2, *result);
		field->addForce(stickForce);
	} else {
		std::cerr << "repulsion\n";
		Force* impulse = new Impulse(base, result->getCrashSpot(), obj1, obj2);
		field->addForce(impulse);
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
