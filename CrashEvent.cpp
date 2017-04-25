#include "CrashEvent.h"
#include "Object.h"
#include "Force.h"
#include "NumberBox.h"
#include "Calculater.h"
#include "CrashResult.h"
#include <iostream>
#include <math.h>

Field::CrashEvent::CrashEvent(void) : Field::Event::Event()
{
}

const char Field::CrashEvent::OVER = 1;
const char Field::CrashEvent::SUCCESS = 0;
const char Field::CrashEvent::FAILURE = -1;

void Field::CrashEvent::exec(void)
{
	for (short i = 0  ;  i < (field->objectNum - 1)  ;  i++) {
		for (short j = i + 1  ;  j < field->objectNum  ;  j++) {

			if (field->object[i]->isActive() | field->object[j]->isActive()) {
				if (canCrashObjSphere(field->object[i], field->object[j])) {

					if (reflectIfCrash(field->object[i], field->object[j])) {
						short result = -1;

						if (field->object[i]->whichClass() == 'N'  &&  field->object[j]->whichClass() == 'O'  &&  j >= 3) {
							std::cerr << j << '\n';
							NumberBox* numberBox = (NumberBox*)field->object[i];
							result = NumberBox::decompose(&numberBox, &(field->object[j]));
							field->object[i] = numberBox;
							std::cerr << "change end\n";
							field->reportScore(result);
						}
						if (field->object[i]->whichClass() == 'O'  &&  field->object[j]->whichClass() == 'N'  &&  i >= 3) {
							std::cerr << i << '\n';
							NumberBox* numberBox = (NumberBox*)field->object[j];
							result = NumberBox::decompose(&numberBox, &(field->object[i]));
							field->object[j] = numberBox;
							std::cerr << "change end\n";
							field->reportScore(result);
						}

					}
				}
			}

		}
	}
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
	if (result.getResult() == true) {
		reflectPlgnAndVrtx(obj1, obj2, &result);
//		std::cerr << "A";
		return true;
	}

	judgePlgnAndVrtx(obj2, obj1, &result);
	if (result.getResult() == true) {
		reflectPlgnAndVrtx(obj2, obj1, &result);
//		std::cerr << "B";
		return true;
	}

	judgeLineAndLine(obj1, obj2, &result);
	if (result.getResult() == true) {
		reflectLineAndLine(obj1, obj2, &result);
//		std::cerr << "C";
		return true;
	}

	return false;
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

		for (short i = 0  ;  i < plgnNum  ;  i++) {

			if (objPlgn->isPolygonEmbody(i) == false) continue;

			Vector solution;

			if (Calculater::solveCubicEquation(
				objPlgn->getPolygon2Vertex(i) - objPlgn->getPolygon1Vertex(i),
				objPlgn->getPolygon3Vertex(i) - objPlgn->getPolygon1Vertex(i),
				relativeVelocity,
				objVrtx->getVertex(j) - objPlgn->getPolygon1Vertex(i),
				&solution
			)) {
				if (0 <= solution.getX()  &&  0 <= solution.getY()  &&  solution.getX() + solution.getY() <= 1) {
					if (0 <= solution.getZ()  &&  solution.getZ() < 1) {
						result->setPlgnIdx(i);
						result->setVrtxIdx(j);
						result->setCrashSpot(objVrtx->getVertex(j));
						result->setResult(true);
						return;
					}
				}
			}
		}
	}

	result->setResult(false);
}

void Field::CrashEvent::judgeLineAndLine(Object* obj1, Object* obj2, CrashResult* result)
{
	short lineNum1 = obj1->getLineNum();
	short lineNum2 = obj2->getLineNum();

	Vector relativeVelocity = obj1->getVelocity() - obj2->getVelocity();

	for (short i = 0  ;  i < lineNum1  ;  i++) {
		Vector obj1Line = obj1->getLineLVertex(i) - obj1->getLineRVertex(i);

		for (short j = 0  ;  j < lineNum2  ;  j++) {
			Vector solution;

			if (Calculater::solveCubicEquation(
				relativeVelocity,
				obj1Line,
				obj2->getLineRVertex(j) - obj2->getLineLVertex(j),
				obj2->getLineRVertex(j) - obj1->getLineRVertex(i),
				&solution
			)) {
				if ((0 <= solution.getY()  &&  solution.getY() <= 1)  &&  (0 <= solution.getZ()  &&  solution.getZ() <= 1)) {
					if (0 <= solution.getX()  &&  solution.getX() < 1) {
						Vector crashSpot = (obj1->getLineRVertex(i) * (1 - solution.getY())) + (obj1->getLineLVertex(i) * solution.getY());
						result->setLine1Idx(i);
						result->setLine2Idx(j);
						result->setCrashSpot(crashSpot);
						result->setResult(true);
						return;
					}
				}

			}
		}
	}

	result->setResult(false);
}

void Field::CrashEvent::reflectPlgnAndVrtx(Object* objPlgn, Object* objVrtx, CrashResult* result)
{
	short plgnIdx = result->getPlgnIdx();

	calcRepulsion(
		objPlgn,
		objVrtx,
		objPlgn->getPolygon2Vertex(plgnIdx) - objPlgn->getPolygon1Vertex(plgnIdx),
		objPlgn->getPolygon3Vertex(plgnIdx) - objPlgn->getPolygon1Vertex(plgnIdx),
		result
	);
}

void Field::CrashEvent::reflectLineAndLine(Object* obj1, Object* obj2, CrashResult* result)
{
	short idx1 = result->getLine1Idx();
	short idx2 = result->getLine2Idx();

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
	Vector n;

	if (Calculater::calculate1(p, q, &n) == false) {
		if (Calculater::calculate1(q, p, &n) == false) {
			n.setVector(2, 2, 2);

			if (p.getX() == 0  &&  p.getY() == 0)
				n.setZ(0);
			if (p.getX() == 0  &&  p.getZ() == 0)
				n.setY(0);
			if (p.getY() == 0  &&  p.getZ() == 0)
				n.setX(0);

			if (q.getX() == 0  &&  q.getY() == 0)
				n.setZ(0);
			if (q.getX() == 0  &&  q.getZ() == 0)
				n.setY(0);
			if (q.getY() == 0  &&  q.getZ() == 0)
				n.setX(0);

			if (n.getX() == 0  &&  n.getY() == 0)
				n.setZ(1);
			if (n.getX() == 0  &&  n.getZ() == 0)
				n.setY(1);
			if (n.getY() == 0  &&  n.getZ() == 0)
				n.setX(1);

			if (n.getX() + n.getY() + n.getZ() >= 1.9) {
				std::cerr << "reflection failed...   code:" << n.getX() + n.getY() + n.getZ() << '\n';
				return;
			} else {
//				std::cerr << "reflection one chance\n";
			}
		}
	}

	Vector solution;

	if (Calculater::solveCubicEquation(p, q, n, (obj1->getVelocity() - obj2->getVelocity()), &solution)) {
		float m1 = obj1->getMass();
		float m2 = obj2->getMass();

		Vector vector = n * solution.getZ() * 2.0 * (m1 * m2 / (m1 + m2));

		Force* force = new Force(vector, result->getCrashSpot(), obj2, obj1);
		field->addForce(force);
	} else {
		std::cerr << "through?\n";
	}
}
