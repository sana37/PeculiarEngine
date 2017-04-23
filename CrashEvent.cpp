#include "CrashEvent.h"
#include "Object.h"
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
/*
					if (field->stateCrash[i][j] == '\0') {
						field->object[i]->back();
						judgeCrash(
							field->object[i],
							field->object[j],
							&(field->stateCrash[i][j]),
							&(field->indexCrash[i][j]),
							&(field->indexCrash[j][i])
						);
						field->object[i]->run();
					}
*/
/*
					judgeCrash(
						field->object[i],
						field->object[j],
						&(field->stateCrash[i][j]),
						&(field->indexCrash[i][j]),
						&(field->indexCrash[j][i])
					);
*/


//					if ('A' <= field->stateCrash[i][j]  &&  field->stateCrash[i][j] <= 'Z') {//syokiiti ga kantsu!!! notoki through
					if (execReflection(field->object[i], field->object[j])) {
						short result = -1;

//						std::cerr << i << " " << j << " reflect\n";
/*
						reflect(
							field->object[i],
							field->object[j],
							field->stateCrash[i][j],
							field->indexCrash[i][j],
							field->indexCrash[j][i]
						);
*/
//						reflect(field->object[i], field->object[j], state, idx1, idx2);
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

						field->object[i]->run();//oukyuu syoti
						field->object[j]->run();//honntoha yokunai

						for (short k = 0  ;  k < field->objectNum  ;  k++) {
							if (k < i)
								field->stateCrash[k][i] = '\0';
							else if (i < k)
								field->stateCrash[i][k] = '\0';

							if (k < j)
								field->stateCrash[k][j] = '\0';
							else if (j < k)
								field->stateCrash[j][k] = '\0';
						}
					}
				} else {
					field->stateCrash[i][j] = '\0';
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

bool Field::CrashEvent::execReflection(Object* obj1, Object* obj2)
{
	CrashResult result;

	judgePlgnAndVrtxNeo(obj1, obj2, &result);
	if (result.getResult() == true) {
		reflectPlgnAndVrtx(obj1, obj2, &result);
		return true;
	}

	judgePlgnAndVrtxNeo(obj2, obj1, &result);
	if (result.getResult() == true) {
		reflectPlgnAndVrtx(obj2, obj1, &result);
		return true;
	}

	judgeLineAndLineNeo(obj1, obj2, &result);
	if (result.getResult() == true) {
		reflectLineAndLine(obj1, obj2, &result);
		return true;
	}

	return false;
}

void Field::CrashEvent::judgePlgnAndVrtxNeo(Object* objPlgn, Object* objVrtx, CrashResult* result)
{
	short plgnNum = objPlgn->getPolygonNum();
	short vrtxNum = objVrtx->getVertexNum();

	Vector relativeVelocity = objPlgn->getVelocity() - objVrtx->getVelocity();
	float A[3], B[3], V[3], P[3], S[3];


	relativeVelocity.getVector(V);

	for (short j = 0  ;  j < vrtxNum  ;  j++) {

		if (objVrtx->isVertexEmbody(j) == false) continue;
		if (canCrashObjSphereAndVrtx(objPlgn, objVrtx->getVertex(j)) == false) {
			std::cerr << '.';
			continue;
		} else {
			std::cerr << '_';
		}

		for (short i = 0  ;  i < plgnNum  ;  i++) {

			if (objPlgn->isPolygonEmbody(i) == false) continue;

			(objPlgn->getPolygon2Vertex(i) - objPlgn->getPolygon1Vertex(i)).getVector(A);
			(objPlgn->getPolygon3Vertex(i) - objPlgn->getPolygon1Vertex(i)).getVector(B);

			(objVrtx->getVertex(j) - objPlgn->getPolygon1Vertex(i)).getVector(P);

			if (Calculater::solveCubicEquation(A, B, V, P, S)) {
				if (0 <= S[0]  &&  0 <= S[1]  &&  S[0] + S[1] <= 1) {
					if (-1 < S[2]  &&  S[2] <= 0) {
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

void Field::CrashEvent::judgeLineAndLineNeo(Object* obj1, Object* obj2, CrashResult* result)//crashResult's name space ?
{
	short lineNum1 = obj1->getLineNum();
	short lineNum2 = obj2->getLineNum();

	float V[3], P[3], Q[3], R[3], S[3];


	(obj1->getVelocity() - obj2->getVelocity()).getVector(V);

	for (short i = 0  ;  i < lineNum1  ;  i++) {

		if (!canCrashObjSphereAndVrtx(obj2, obj1->getLineLVertex(i))  &&  !canCrashObjSphereAndVrtx(obj2, obj1->getLineRVertex(i))) {
			std::cerr << '.';
			continue;
		} else {
			std::cerr << '_';
		}

		(obj1->getLineLVertex(i) - obj1->getLineRVertex(i)).getVector(P);

		for (short j = 0  ;  j < lineNum2  ;  j++) {

			if (!canCrashObjSphereAndVrtx(obj1, obj2->getLineLVertex(j))  &&  !canCrashObjSphereAndVrtx(obj1, obj2->getLineRVertex(j))) {
				std::cerr << '\'';
				continue;
			} else {
				std::cerr << '\"';
			}

			(obj2->getLineRVertex(j) - obj2->getLineLVertex(j)).getVector(Q);
			(obj2->getLineRVertex(j) - obj1->getLineRVertex(i)).getVector(R);

			if (Calculater::solveCubicEquation(V, P, Q, R, S)) {
				if ((0 <= S[1]  &&  S[1] <= 1)  &&  (0 <= S[2]  &&  S[2] <= 1)) {
					if (-1 < S[0]  &&  S[0] <= 0) {
						Vector crashSpot = (obj1->getLineRVertex(i) * (1 - S[1])) + (obj1->getLineLVertex(i) * S[1]);
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
/*
void Field::CrashEvent::judgeCrash(Object* object1, Object* object2, char* state_p, short* index1_p, short* index2_p)
{
	CrashResult result;
	float distMin = 0;
	bool distMinFirstFlag = true;

	switch (*state_p) {
		case '\0': {
			break;
		}
		case 'a' : {
			result = judgePlgnAndVrtx(object1, object2, index1_p, index2_p, true);
			if (result.getResult() == OVER)
				*state_p = 'A';
			return;
		}
		case 'b' : {
			result = judgePlgnAndVrtx(object2, object1, index2_p, index1_p, true);
			if (result.getResult() == OVER)
				*state_p = 'B';
			return;
		}
		case 'c' : {
			result = judgeLineAndLine(object1, object2, index1_p, index2_p, true);
			if (result.getResult() == OVER)
				*state_p = 'C';
			return;
		}
		case '*' : {
			*state_p = '*';//kaiten wo kouryo suruto koreha dame
			return;
		}
	}

	*state_p = '*';

	result = judgePlgnAndVrtx(object1, object2, index1_p, index2_p, false);

	if (result.getResult() == SUCCESS) {
		distMin = result.getDistMin();
		distMinFirstFlag = false;
		*state_p = 'a';
	}

	result = judgePlgnAndVrtx(object2, object1, index2_p, index1_p, false);

	if (result.getResult() == SUCCESS  &&  (distMinFirstFlag  ||  result.getDistMin() < distMin)) {
		distMin = result.getDistMin();
		distMinFirstFlag = false;
		*state_p = 'b';
	}

	result = judgeLineAndLine(object1, object2, index1_p, index2_p, false);

	if (result.getResult() == SUCCESS  &&  (distMinFirstFlag  ||  result.getDistMin() < distMin)) {
		*state_p = 'c';
	}

}
/////////////////////////index1_p and index2_p order???
Field::CrashEvent::CrashResult Field::CrashEvent::judgePlgnAndVrtx(
	Object* objPlgn,
	Object* objVrtx,
	short* idxPlgn_p,
	short* idxVrtx_p,
	bool skip)
{
	CrashResult result;
	bool distMinFirstFlag = true;
	float distMin = 0;

	short plgnNum = objPlgn->getPolygonNum();
	short vrtxNum = objVrtx->getVertexNum();

	Vector relativeVelocity = objPlgn->getVelocity() - objVrtx->getVelocity();
	float A[3], B[3], V[3], P[3], S[3];


	relativeVelocity.getVector(V);

	for (short i = 0  ;  i < plgnNum  ;  i++) {

		if (skip  &&  *idxPlgn_p != i) continue;
		if (objPlgn->isPolygonEmbody(i) == false) continue;

		(objPlgn->getPolygon2Vertex(i) - objPlgn->getPolygon1Vertex(i)).getVector(A);
		(objPlgn->getPolygon3Vertex(i) - objPlgn->getPolygon1Vertex(i)).getVector(B);

		for (short j = 0  ;  j < vrtxNum  ;  j++) {

			if (skip  &&  *idxVrtx_p != j) continue;
			if (objVrtx->isVertexEmbody(j) == false) continue;

			(objVrtx->getVertex(j) - objPlgn->getPolygon1Vertex(i)).getVector(P);

			if (Calculater::solveCubicEquation(A, B, V, P, S)) {
				if (skip == false) {
					if (0 <= S[0]  &&  0 <= S[1]  &&  S[0] + S[1] <= 1) {
						if ((S[2] < distMin  ||  distMinFirstFlag == true)  &&  S[2] > 0) {
							distMin = S[2];
							distMinFirstFlag = false;
							*idxPlgn_p = i;
							*idxVrtx_p = j;
						}
						if (S[2] <= 0) {
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						}
					}
				} else {
					if (S[2] <= 0) {
						result.setResult(OVER);
						result.setDistMin(0);	//naosu
						return result;
					} else {
						result.setResult(SUCCESS);
						result.setDistMin(S[2]);
						return result;
					}
				}
			}
		}
	}

	result.setResult(distMinFirstFlag ? FAILURE : SUCCESS);
	result.setDistMin(distMin);
	return result;
}

Field::CrashEvent::CrashResult Field::CrashEvent::judgeLineAndLine(Object* obj1, Object* obj2, short* idx1_p, short* idx2_p, bool skip)
{
	CrashResult result;
	bool distMinFirstFlag = true;
	float distMin = 0;

	short lineNum1 = obj1->getLineNum();
	short lineNum2 = obj2->getLineNum();

	Vector relativeVelocity = obj1->getVelocity() - obj2->getVelocity();
	float V[3], P[3], Q[3], R[3], S[3];


	relativeVelocity.getVector(V);

	for (short i = 0  ;  i < lineNum1  ;  i++) {

		if (skip  &&  *idx1_p != i) continue;

		(obj1->getLineLVertex(i) - obj1->getLineRVertex(i)).getVector(P);

		for (short j = 0  ;  j < lineNum2  ;  j++) {

			if (skip  &&  *idx2_p != j) continue;

			(obj2->getLineRVertex(j) - obj2->getLineLVertex(j)).getVector(Q);
			(obj2->getLineRVertex(j) - obj1->getLineRVertex(i)).getVector(R);////////////////////////////

			if (Calculater::solveCubicEquation(V, P, Q, R, S)) {
				if (skip == false) {
					if ((0 <= S[1]  &&  S[1] <= 1)  &&  (0 <= S[2]  &&  S[2] <= 1)) {
						if ((S[0] < distMin  ||  distMinFirstFlag == true)  &&  S[0] > 0) {
							distMin = S[0];
							distMinFirstFlag = false;

							*idx1_p = i;
							*idx2_p = j;
						}
					}
				} else {
					if (S[0] <= 0) {
						result.setResult(OVER);
						result.setDistMin(0);
						return result;
					} else {
						result.setResult(SUCCESS);
						result.setDistMin(S[0]);
						return result;
					}
				}

			}
		}
	}

	result.setResult(distMinFirstFlag ? FAILURE : SUCCESS);
	result.setDistMin(distMin);
	return result;
}
*/
void Field::CrashEvent::reflectPlgnAndVrtx(Object* objPlgn, Object* objVrtx, CrashResult* result)
{
	float V[3], P[3], Q[3], n[3], coefficient[3];
	short plgnIdx = result->getPlgnIdx();

	(objPlgn->getPolygon2Vertex(plgnIdx) - objPlgn->getPolygon1Vertex(plgnIdx)).getVector(P);
	(objPlgn->getPolygon3Vertex(plgnIdx) - objPlgn->getPolygon1Vertex(plgnIdx)).getVector(Q);

	if (calculate1(P, Q, n) == false) {
		if (calculate1(Q, P, n) == false) {
			n[0] = 2;
			n[1] = 2;
			n[2] = 2;

			if (P[0] == 0  &&  P[1] == 0)
				n[2] = 0;
			if (P[0] == 0  &&  P[2] == 0)
				n[1] = 0;
			if (P[1] == 0  &&  P[2] == 0)
				n[0] = 0;

			if (Q[0] == 0  &&  Q[1] == 0)
				n[2] = 0;
			if (Q[0] == 0  &&  Q[2] == 0)
				n[1] = 0;
			if (Q[1] == 0  &&  Q[2] == 0)
				n[0] = 0;

			if (n[0] == 0  &&  n[1] == 0)
				n[2] = 1;
			if (n[0] == 0  &&  n[2] == 0)
				n[1] = 1;
			if (n[1] == 0  &&  n[2] == 0)
				n[0] = 1;

			if (n[0] + n[1] + n[2] >= 1.9) {
				std::cerr << "reflection failed...   code:" << n[0] + n[1] + n[2] << '\n';
				return;
			} else {
//				std::cerr << "reflection one chance\n";
			}
		}
	}

	for (short i = 0  ;  i < 2  ;  i++) {
		Vector vector;

		switch (i) {
			case 0 : vector = objPlgn->getVelocity();	break;
			case 1 : vector = objVrtx->getVelocity();	break;
		}
		vector.getVector(V);

		if (Calculater::solveCubicEquation(P, Q, n, V, coefficient)) {
			V[0] -= 2.0 * coefficient[2] * n[0];
			V[1] -= 2.0 * coefficient[2] * n[1];
			V[2] -= 2.0 * coefficient[2] * n[2];

			vector.setVector(V);
			switch (i) {
				case 0 : objPlgn->setVelocity(vector);	break;
				case 1 : objVrtx->setVelocity(vector);	break;
			}
		}
	}
}

void Field::CrashEvent::reflectLineAndLine(Object* obj1, Object* obj2, CrashResult* result)
{
	float V[3], P[3], Q[3], n[3], coefficient[3];
	short idx1 = result->getLine1Idx();
	short idx2 = result->getLine2Idx();

	(obj1->getLineLVertex(idx1) - obj1->getLineRVertex(idx1)).getVector(P);
	(obj2->getLineLVertex(idx2) - obj2->getLineRVertex(idx2)).getVector(Q);

	if (calculate1(P, Q, n) == false) {
		if (calculate1(Q, P, n) == false) {
			n[0] = 2;
			n[1] = 2;
			n[2] = 2;

			if (P[0] == 0  &&  P[1] == 0)
				n[2] = 0;
			if (P[0] == 0  &&  P[2] == 0)
				n[1] = 0;
			if (P[1] == 0  &&  P[2] == 0)
				n[0] = 0;

			if (Q[0] == 0  &&  Q[1] == 0)
				n[2] = 0;
			if (Q[0] == 0  &&  Q[2] == 0)
				n[1] = 0;
			if (Q[1] == 0  &&  Q[2] == 0)
				n[0] = 0;

			if (n[0] == 0  &&  n[1] == 0)
				n[2] = 1;
			if (n[0] == 0  &&  n[2] == 0)
				n[1] = 1;
			if (n[1] == 0  &&  n[2] == 0)
				n[0] = 1;

			if (n[0] + n[1] + n[2] >= 1.9) {
				std::cerr << "reflection failed...   code:" << n[0] + n[1] + n[2] << '\n';
				return;
			} else {
//				std::cerr << "reflection one chance\n";
			}
		}
	}

	for (short i = 0  ;  i < 2  ;  i++) {
		Vector vector;

		switch (i) {
			case 0 : vector = obj1->getVelocity();	break;
			case 1 : vector = obj2->getVelocity();	break;
		}
		vector.getVector(V);

		if (Calculater::solveCubicEquation(P, Q, n, V, coefficient)) {
			V[0] -= 2.0 * coefficient[2] * n[0];
			V[1] -= 2.0 * coefficient[2] * n[1];
			V[2] -= 2.0 * coefficient[2] * n[2];

			vector.setVector(V);
			switch (i) {
				case 0 : obj1->setVelocity(vector);	break;
				case 1 : obj2->setVelocity(vector);	break;
			}
		}
	}

}
/*
void Field::CrashEvent::reflect(Object* obj1, Object* obj2, char state, short idx1, short idx2)
{
	float V[3], P[3], Q[3], n[3], coefficient[3];

	switch (state) {
		case 'A' : {
			(obj1->getPolygon2Vertex(idx1) - obj1->getPolygon1Vertex(idx1)).getVector(P);
			(obj1->getPolygon3Vertex(idx1) - obj1->getPolygon1Vertex(idx1)).getVector(Q);

			break;
		}
		case 'B' : {
			(obj2->getPolygon2Vertex(idx2) - obj2->getPolygon1Vertex(idx2)).getVector(P);
			(obj2->getPolygon3Vertex(idx2) - obj2->getPolygon1Vertex(idx2)).getVector(Q);

			break;
		}
		case 'C' : {
			(obj1->getLineLVertex(idx1) - obj1->getLineRVertex(idx1)).getVector(P);
			(obj2->getLineLVertex(idx2) - obj2->getLineRVertex(idx2)).getVector(Q);

			break;
		}
	}

	if (calculate1(P, Q, n) == false) {
		if (calculate1(Q, P, n) == false) {
			n[0] = 2;
			n[1] = 2;
			n[2] = 2;

			if (P[0] == 0  &&  P[1] == 0)
				n[2] = 0;
			if (P[0] == 0  &&  P[2] == 0)
				n[1] = 0;
			if (P[1] == 0  &&  P[2] == 0)
				n[0] = 0;

			if (Q[0] == 0  &&  Q[1] == 0)
				n[2] = 0;
			if (Q[0] == 0  &&  Q[2] == 0)
				n[1] = 0;
			if (Q[1] == 0  &&  Q[2] == 0)
				n[0] = 0;

			if (n[0] == 0  &&  n[1] == 0)
				n[2] = 1;
			if (n[0] == 0  &&  n[2] == 0)
				n[1] = 1;
			if (n[1] == 0  &&  n[2] == 0)
				n[0] = 1;

			if (n[0] + n[1] + n[2] >= 1.9) {
				std::cerr << "reflection failed...   code:" << n[0] + n[1] + n[2] << '\n';
				return;
			} else {
//				std::cerr << "reflection one chance\n";
			}
		}
	}

	for (short i = 0  ;  i < 2  ;  i++) {
		Vector vector;

		switch (i) {
			case 0 : vector = obj1->getVelocity();	break;
			case 1 : vector = obj2->getVelocity();	break;
		}
		vector.getVector(V);

		if (Calculater::solveCubicEquation(P, Q, n, V, coefficient)) {
			V[0] -= 2.0 * coefficient[2] * n[0];
			V[1] -= 2.0 * coefficient[2] * n[1];
			V[2] -= 2.0 * coefficient[2] * n[2];

			vector.setVector(V);
			switch (i) {
				case 0 : obj1->setVelocity(vector);	break;
				case 1 : obj2->setVelocity(vector);	break;
			}
		}
	}

}
*/
bool Field::CrashEvent::calculate1(float* a, float* b, float* ans)
{
	float temp1, temp2;
	short s = 0;
	short t = 1;
	short u = 2;

	for (short i = 0  ;  i < 6  ;  i++) {
		switch (i) {
			case 1 : {
				s = 0;
				t = 2;
				u = 1;
				break;
			}
			case 2 : {
				s = 1;
				t = 0;
				u = 2;
				break;
			}
			case 3 : {
				s = 1;
				t = 2;
				u = 0;
				break;
			}
			case 4 : {
				s = 2;
				t = 0;
				u = 1;
				break;
			}
			case 5 : {
				s = 2;
				t = 1;
				u = 0;
				break;
			}
		}
		if (a[s] == 0  ||  a[t] == 0  ||  (b[s] - a[s]/a[t]*b[t]) == 0) {
//			std::cerr << "ERROR : reflect     ";
			if (i == 5) {
//				std::cerr << "sad...\n";
				return false;
			}
		} else {
			break;
		}
	}

	temp1 = (a[u]/a[t]*b[t] - b[u]) / (b[s] - a[s]/a[t]*b[t]);
	temp2 = (a[u]/a[s]*b[s] - b[u]) / (b[t] - a[t]/a[s]*b[s]);

	ans[u] = sqrt(1 / (temp1*temp1 + temp2*temp2 + 1));//z
	ans[s] = temp1 * ans[2];//x
	ans[t] = temp2 * ans[2];//y

	return true;
}
