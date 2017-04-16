#include "CrashEvent.h"
#include "Object.h"
#include "NumberBox.h"
#include "Calculater.h"
#include <iostream>
#include <math.h>

Field::CrashEvent::CrashEvent(void) : Field::Event::Event()
{
}

void Field::CrashEvent::exec(void)
{
	for (short i = 0  ;  i < (field->objectNum - 1)  ;  i++) {
		for (short j = i + 1  ;  j < field->objectNum  ;  j++) {
			if (field->object[i]->isActive() | field->object[j]->isActive()) {
				if (interference(i , j)) {
/*
					if (field->stateCrash[i][j] == '\0') {
						field->object[i]->back();
						field->stateCrash[i][j] = judgeCrash(i , j);
						field->object[i]->move();
					}
*/
					field->stateCrash[i][j] = judgeCrash(i , j);
//					if (i == 1  &&  j == 3)
//						std::cerr << "j = " << j << " , state : " << field->stateCrash[i][j] << '\n';

					if ('A' <= field->stateCrash[i][j]  &&  field->stateCrash[i][j] <= 'Z') {//syokiiti ga kantsu!!! notoki through
						short result = -1;
//						std::cout << "reflect:" << i << " , " << j << '\n';
						reflect(i , j);
						if (field->object[i]->whichClass() == 'N'  &&  field->object[j]->whichClass() == 'O'  &&  j >= 3) {
							std::cerr << j << '\n';
							NumberBox* numberBox = (NumberBox*)field->object[i];
							result = NumberBox::decompose(&numberBox , &(field->object[j]));
							field->object[i] = numberBox;
							std::cerr << "change end\n";
							field->reportScore(result);
						}
						if (field->object[i]->whichClass() == 'O'  &&  field->object[j]->whichClass() == 'N'  &&  i >= 3) {
							std::cerr << i << '\n';
							NumberBox* numberBox = (NumberBox*)field->object[j];
							result = NumberBox::decompose(&numberBox , &(field->object[i]));
							field->object[j] = numberBox;
							std::cerr << "change end\n";
							field->reportScore(result);
						}

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

bool Field::CrashEvent::interference(short num1 , short num2)
{
	Vector dist = field->object[num1]->getGravityCenter() - field->object[num2]->getGravityCenter();
	Vector relativeVelocity = field->object[num1]->getVelocity() - field->object[num2]->getVelocity();

	return dist.getMagnitude() <= (field->object[num1]->getRadius() + field->object[num2]->getRadius() + relativeVelocity.getMagnitude());
}

char Field::CrashEvent::judgeCrash(short num1 , short num2)
{
	char state = '*';
	float distMin = 0;
	bool distMinFirstFlag = true;
	Vector relativeVelocity;

	short lineNum1 , lineNum2;

	if (field->stateCrash[num1][num2] == '*') return '*';

	relativeVelocity = field->object[num1]->getVelocity() - field->object[num2]->getVelocity();

	for (short h = 0  ;  h < 2  ;  h++) {
		Object* objectPolygon;
		Object* objectVertex;

		short polygonNum;
		short vertexNum;
		float A[3] , B[3] , V[3] , P[3] , S[3];

		if (field->stateCrash[num1][num2] == 'c') break;
		if ((h == 0  &&  field->stateCrash[num1][num2] == 'b')  ||  (h == 1  &&  field->stateCrash[num1][num2] == 'a')) {
			relativeVelocity.multiply(-1);
			continue;
		}

		switch (h) {
			case 0 : {
				objectPolygon = field->object[num1];
				objectVertex = field->object[num2];
				break;
			}
			case 1 : {
				objectPolygon = field->object[num2];
				objectVertex = field->object[num1];
				break;
			}
		}

		polygonNum = objectPolygon->getPolygonNum();
		vertexNum = objectVertex->getVertexNum();

		relativeVelocity.getVector(V);

		for (short i = 0  ;  i < polygonNum  ;  i++) {
			switch (field->stateCrash[num1][num2]) {
				case 'a' : {//-> object1 : polygon , object2 : vertex
					if (field->indexCrash[num1][num2] != i) continue;//h = 0
					break;
				}
				case 'b' : {//-> object1 : vertex  , object2 : polygon
					if (field->indexCrash[num2][num1] != i) continue;//h = 1
					break;
				}
			}
			if (objectPolygon->isPolygonEmbody(i) == false)
				continue;

			(objectPolygon->getPolygon2Vertex(i) - objectPolygon->getPolygon1Vertex(i)).getVector(A);
			(objectPolygon->getPolygon3Vertex(i) - objectPolygon->getPolygon1Vertex(i)).getVector(B);

			for (short j = 0  ;  j < vertexNum  ;  j++) {
				switch (field->stateCrash[num1][num2]) {
					case 'a' : {
						if (field->indexCrash[num2][num1] != j) continue;
						break;
					}
					case 'b' : {
						if (field->indexCrash[num1][num2] != j) continue;
						break;
					}
				}
				if (objectVertex->isVertexEmbody(j) == false)
					continue;

				(objectVertex->getVertex(j) - objectPolygon->getPolygon1Vertex(i)).getVector(P);

				if (Calculater::solveCubicEquation(A , B , V , P , S)) {
					if (field->stateCrash[num1][num2] == '\0') {
						if (0 <= S[0]  &&  0 <= S[1]  &&  S[0] + S[1] <= 1) {
							if ((S[2] < distMin  ||  distMinFirstFlag == true)  &&  S[2] > 0) {
								distMin = S[2];
								distMinFirstFlag = false;
								switch (h) {
									case 0 : {
										field->indexCrash[num1][num2] = i;
										field->indexCrash[num2][num1] = j;
										state = 'a';
										break;
									}
									case 1 : {
										field->indexCrash[num1][num2] = j;
										field->indexCrash[num2][num1] = i;
										state = 'b';
										break;
									}
								}
							}
						}
					} else {
						if (S[2] <= 0) {
							switch (h) {
								case 0 : return 'A';
								case 1 : return 'B';
							}
						} else {
							switch (h) {
								case 0 : return 'a';
								case 1 : return 'b';
							}
						}
					}
				}
			}
		}

		relativeVelocity.multiply(-1);
	}

	lineNum1 = field->object[num1]->getLineNum();
	lineNum2 = field->object[num2]->getLineNum();

	for (short i = 0  ;  i < lineNum1  ;  i++) {
		float V[3] , P[3] , Q[3] , R[3] , S[3];

		if (field->stateCrash[num1][num2] == 'c'  &&  field->indexCrash[num1][num2] != i) continue;


		relativeVelocity.getVector(V);
		(field->object[num1]->getLineLVertex(i) - field->object[num1]->getLineRVertex(i)).getVector(P);

		for (short j = 0  ;  j < lineNum2  ;  j++) {
			if (field->stateCrash[num1][num2] == 'c'  &&  field->indexCrash[num2][num1] != j) continue;

			(field->object[num2]->getLineRVertex(j) - field->object[num2]->getLineLVertex(j)).getVector(Q);
			(field->object[num2]->getLineRVertex(j) - field->object[num1]->getLineRVertex(i)).getVector(R);////////////////////////////

			if (Calculater::solveCubicEquation(V , P , Q , R , S)) {
				if (field->stateCrash[num1][num2] == '\0') {
					if ((0 <= S[1]  &&  S[1] <= 1)  &&  (0 <= S[2]  &&  S[2] <= 1)) {
						if ((S[0] < distMin  ||  distMinFirstFlag == true)  &&  S[0] > 0) {
							distMin = S[0];
							distMinFirstFlag = false;

							field->indexCrash[num1][num2] = i;
							field->indexCrash[num2][num1] = j;
							state = 'c';
						}
					}
				} else {
					if (S[0] <= 0)
						return 'C';
					else
						return 'c';
				}
			}
		}
	}

	return state;
}

void Field::CrashEvent::reflect(short formerNum , short latterNum)
{
	float V[3] , P[3] , Q[3] , n[3] , coefficient[3];

	switch (field->stateCrash[formerNum][latterNum]) {
		case 'A' : {
			(field->object[formerNum]->getPolygon2Vertex(field->indexCrash[formerNum][latterNum]) - field->object[formerNum]->getPolygon1Vertex(field->indexCrash[formerNum][latterNum]))
				.getVector(P);
			(field->object[formerNum]->getPolygon3Vertex(field->indexCrash[formerNum][latterNum]) - field->object[formerNum]->getPolygon1Vertex(field->indexCrash[formerNum][latterNum]))
				.getVector(Q);

			break;
		}
		case 'B' : {
			(field->object[latterNum]->getPolygon2Vertex(field->indexCrash[latterNum][formerNum]) - field->object[latterNum]->getPolygon1Vertex(field->indexCrash[latterNum][formerNum]))
				.getVector(P);
			(field->object[latterNum]->getPolygon3Vertex(field->indexCrash[latterNum][formerNum]) - field->object[latterNum]->getPolygon1Vertex(field->indexCrash[latterNum][formerNum]))
				.getVector(Q);

			break;
		}
		case 'C' : {
			(field->object[formerNum]->getLineLVertex(field->indexCrash[formerNum][latterNum]) - field->object[formerNum]->getLineRVertex(field->indexCrash[formerNum][latterNum]))
				.getVector(P);
			(field->object[latterNum]->getLineLVertex(field->indexCrash[latterNum][formerNum]) - field->object[latterNum]->getLineRVertex(field->indexCrash[latterNum][formerNum]))
				.getVector(Q);

			break;
		}
	}

	if (calculate1(P , Q , n) == false) {
		if (calculate1(Q , P , n) == false) {
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
		short index;
		Vector temp;

		switch (i) {
			case 0 : index = formerNum;	break;
			case 1 : index = latterNum;	break;
		}

		temp = field->object[index]->getVelocity();
		temp.getVector(V);

		if (Calculater::solveCubicEquation(P , Q , n , V , coefficient)) {
			V[0] -= 2.0 * coefficient[2] * n[0];
			V[1] -= 2.0 * coefficient[2] * n[1];
			V[2] -= 2.0 * coefficient[2] * n[2];

			temp.setVector(V);
			field->object[index]->setVelocity(temp);
		}
	}
}

bool Field::CrashEvent::calculate1(float* a , float* b , float* ans)
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
