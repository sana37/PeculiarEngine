#include "Field.h"
#include "Sight.h"
#include "Object.h"
#include "Player.h"
#include "NumberBox.h"
#include "Gunner.h"
#include <QTimer>
#include <iostream>
#include <math.h>

#define OBJECT_NUM	7
#define TIME_UNIT	10
#define WORLD		80

Field::Field(void)
{
	std::cerr << "Hello world\n";
	objectNum = OBJECT_NUM;/////////////
	time = new QTimer;
	dominatorTime = new QTimer;
	autoGeneration = new QTimer;
	object = new Object*[objectNum];
	object[0] = new Object("ground0");
	object[1] = new Object("sky1");
	object[2] = new Gunner("player" , "bullet");
	object[3] = new NumberBox(54);
	object[4] = new NumberBox(50);
	object[5] = new NumberBox(91);
	object[6] = new NumberBox(72);
	sight = new Sight(object , objectNum , 2);

	stateCrash = new char*[objectNum];
	indexCrash = new short*[objectNum];
	for(short i = 0  ;  i < objectNum  ;  i++){
		std::cerr << "classCode : " << object[i]->whichClass() << '\n';
		stateCrash[i] = new char[objectNum];
		indexCrash[i] = new short[objectNum];
		for(short j = 0  ;  j < objectNum  ;  j++){
			stateCrash[i][j] = '\0';
			indexCrash[i][j] = -1;
		}
	}

	time->start(TIME_UNIT);
	dominatorTime->stop();
	autoGeneration->start(5000);
	QObject::connect(time , SIGNAL(timeout()) , this , SLOT(update()));
	QObject::connect(dominatorTime , SIGNAL(timeout()) , sight , SLOT(moveSight()));
	QObject::connect(sight , SIGNAL(timeCall()) , this , SLOT(timeControl()));
	QObject::connect(autoGeneration , SIGNAL(timeout()) , this , SLOT(autoGenerate()));

	Vector temp;

	temp.setVector(8 , 2 , 0);
	object[3]->teleport(&temp);
	temp.setVector(0.03 , 0.05 , 0);
	object[3]->setVelocity(&temp);

	temp.setVector(6 , 6 , 6);
	object[4]->teleport(&temp);
	temp.setVector(-0.03 , 0.03 , 0.08);
	object[4]->setVelocity(&temp);

	temp.setVector(3 , 3 , 3);
	object[5]->teleport(&temp);
	temp.setVector(0 , -0.05 , 0.03);
	object[5]->setVelocity(&temp);

	temp.setVector(0 , 5 , 0);
	object[6]->teleport(&temp);
	temp.setVector(0.04 , 0.1 , 0);
	object[6]->setVelocity(&temp);

	deadObjectNum = 0;
	autoGenerationIndex = 0;
}

Field::~Field(void)
{
	sight->close();
}

void Field::open(void)
{
	sight->resize(1000 , 700);
	sight->move(400 , 0);
//	sight->showMaximized();
	sight->show();
}

void Field::timeControl(void)
{
	if(time->isActive()){
		time->stop();
		dominatorTime->start(TIME_UNIT);
	}else{
		time->start(TIME_UNIT);
		dominatorTime->stop();
	}
}

void Field::update(void)
{
//	bool velocityChangeFlag[OBJECT_NUM];//
	bool signal;

//stateCrash nado wo initialize
	for(short i = 0  ;  i < objectNum  ;  i++){
/*		if(velocityChangeFlag[i]){
			for(short j = 0  ;  j < OBJECT_NUM  ;  j++){
				if(j < i)
					stateCrash[j][i] = '\0';
				else if(i < j)
					stateCrash[i][j] = '\0';
			}
		}*/
		switch(object[i]->whichClass()){
			case 'G' : {
				Gunner* gunner = (Gunner*)object[i];

				signal = gunner->updateGunner();
				if(signal){
					Object bullet(gunner->fire());
					objectGenerate(&bullet);
				}
				break;
			}
			case 'P' : {
				Player* player = (Player*)object[i];

				signal = player->updatePlayer();
				break;
			}
			case 'O' : {
				signal = object[i]->update();
				break;
			}
			case 'N' : {
				signal = object[i]->update();
				break;
			}
		}
		if(i >= 2){
			if((object[i]->getGravityCenter()).getMagnitude() > WORLD){
//				std::cerr << "deleted\n";
				if(deadObjectNum < 100){
					deadObjectIndex[deadObjectNum] = i;
					deadObjectNum++;
				}
			}
		}
	}
	sight->receiveMovement();
	for(short i = 0  ;  i < (objectNum - 1)  ;  i++){
		for(short j = i + 1  ;  j < objectNum  ;  j++){
			if(object[i]->isActive() | object[j]->isActive()){
				if(interference(i , j)){
/*
					if(stateCrash[i][j] == '\0'){
						object[i]->back();
						stateCrash[i][j] = judgeCrash(i , j);
						object[i]->move();
					}
*/
					stateCrash[i][j] = judgeCrash(i , j);
//					if(i == 1  &&  j == 3)
//						std::cerr << "j = " << j << " , state : " << stateCrash[i][j] << '\n';

					if('A' <= stateCrash[i][j]  &&  stateCrash[i][j] <= 'Z'){//syokiiti ga kantsu!!! notoki through
						short result = -1;
//						std::cout << "reflect:" << i << " , " << j << '\n';
						reflect(i , j);
						if(object[i]->whichClass() == 'N'  &&  object[j]->whichClass() == 'O'  &&  j >= 3){
							std::cerr << j << '\n';
							NumberBox* numberBox = (NumberBox*)object[i];
							result = NumberBox::decompose(&numberBox , &object[j]);
							object[i] = numberBox;
							std::cerr << "change end\n";
							reportScore(result);
						}
						if(object[i]->whichClass() == 'O'  &&  object[j]->whichClass() == 'N'  &&  i >= 3){
							std::cerr << i << '\n';
							NumberBox* numberBox = (NumberBox*)object[j];
							result = NumberBox::decompose(&numberBox , &object[i]);
							object[j] = numberBox;
							std::cerr << "change end\n";
							reportScore(result);
						}

						for(short k = 0  ;  k < objectNum  ;  k++){
							if(k < i)
								stateCrash[k][i] = '\0';
							else if(i < k)
								stateCrash[i][k] = '\0';

							if(k < j)
								stateCrash[k][j] = '\0';
							else if(j < k)
								stateCrash[j][k] = '\0';
						}
					}
				}else{
					stateCrash[i][j] = '\0';
				}
			}
		}
	}
//	std::cerr << "wey\n";
	sight->updateGL();
//	std::cerr << "update end\n";
}

void Field::autoGenerate(void)
{
	Object* temp;
	Vector vector;

	switch(autoGenerationIndex){//atode random ni suru
		case 0 : {
			temp = new NumberBox(94);
			break;
		}
		case 1 : {
			temp = new NumberBox(42);
			break;
		}
		case 2 : {
			temp = new NumberBox(66);
			break;
		}
		case 3 : {
			temp = new NumberBox(81);
			break;
		}
		case 4 : {
			temp = new NumberBox(12);
			break;
		}
		case 5 : {
			temp = new NumberBox(35);
			break;
		}
		case 6 : {
			temp = new NumberBox(48);
			break;
		}
		case 7 : {
			temp = new NumberBox(18);
			break;
		}
		case 8 : {
			temp = new NumberBox(99);
			break;
		}
		case 9 : {
			temp = new NumberBox(6);
			break;
		}
		case 10 : {
			temp = new NumberBox(68);
			break;
		}
		case 11 : {
			temp = new NumberBox(32);
			break;
		}
		default : {
			return;
		}
	}

	switch(autoGenerationIndex % 4){
		case 0 : {
			vector.setVector(25 , 45 , 25);
			break;
		}
		case 1 : {
			vector.setVector(25 , 45 , -25);
			break;
		}
		case 2 : {
			vector.setVector(-25 , 45 , -25);
			break;
		}
		case 3 : {
			vector.setVector(-25 , 45 , 25);
			break;
		}
	}
	temp->teleport(&vector);

	switch(autoGenerationIndex % 3){
		case 0 : {
			vector.setVector(0.02 , 0.04 , 0);
			break;
		}
		case 1 : {
			vector.setVector(-0.01 , 0.01 , 0.04);
			break;
		}
		case 2 : {
			vector.setVector(0 , -0.04 , 0.02);
			break;
		}
	}
	temp->setVelocity(&vector);

	objectGenerate(temp);
	autoGenerationIndex++;
}

void Field::objectGenerate(Object* newObject)
{
	char** tempStateCrash;
	short** tempIndexCrash;
	Object** tempObject;
/*
	if(deadObjectNum > 0){
		deadObjectNum--;
		delete object[deadObjectIndex[deadObjectNum]];
//		object[deadObjectIndex[deadObjectNum]] = newObject;
		switch(newObject->whichClass()){
			case 'O' : object[deadObjectIndex[deadObjectNum]] = new Object(*newObject);	break;
			case 'P' : object[deadObjectIndex[deadObjectNum]] = new Player(*((Player*)newObject));	break;
			case 'G' : object[deadObjectIndex[deadObjectNum]] = new Gunner(*((Gunner*)newObject));	break;
			case 'N' : object[deadObjectIndex[deadObjectNum]] = new NumberBox(*((NumberBox*)newObject));	break;
		}
		std::cerr << "Object generation success??\n";
		return;
	}
*/


	objectNum++;

	tempObject = new Object*[objectNum];
	tempStateCrash = new char*[objectNum];
	tempIndexCrash = new short*[objectNum];
	for(short i = 0  ;  i < objectNum  ;  i++){
		if(i == objectNum - 1){
			switch(newObject->whichClass()){
				case 'O' : tempObject[i] = new Object(*newObject);	break;
				case 'P' : tempObject[i] = new Player(*((Player*)newObject));	break;
				case 'G' : tempObject[i] = new Gunner(*((Gunner*)newObject));	break;
				case 'N' : tempObject[i] = new NumberBox(*((NumberBox*)newObject));	break;
			}
		}else{
			switch(object[i]->whichClass()){
				case 'O' : tempObject[i] = new Object(*object[i]);	break;
				case 'P' : tempObject[i] = new Player(*((Player*)object[i]));	break;
				case 'G' : tempObject[i] = new Gunner(*((Gunner*)object[i]));	break;
				case 'N' : tempObject[i] = new NumberBox(*((NumberBox*)object[i]));	break;
			}
		}
		tempStateCrash[i] = new char[objectNum];
		tempIndexCrash[i] = new short[objectNum];
		for(short j = 0  ;  j < objectNum  ;  j++){
			if(i == objectNum - 1  ||  j == objectNum - 1){
				tempStateCrash[i][j] = '\0';
				tempIndexCrash[i][j] = -1;
			}else{
				tempStateCrash[i][j] = stateCrash[i][j];
				tempIndexCrash[i][j] = indexCrash[i][j];
			}
		}
	}

	for(short i = 0  ;  i < objectNum - 1  ;  i++)
		delete object[i];
	delete[] object;
	delete[] stateCrash;
	delete[] indexCrash;

	object = tempObject;
	stateCrash = tempStateCrash;
	indexCrash = tempIndexCrash;

	sight->updateObject(object , objectNum);
}

bool Field::interference(short num1 , short num2)
{
	Vector dist = object[num1]->getGravityCenter() - object[num2]->getGravityCenter();
	Vector relativeVelocity = object[num1]->getVelocity() - object[num2]->getVelocity();

	return(dist.getMagnitude() <= (object[num1]->getRadius() + object[num2]->getRadius() + relativeVelocity.getMagnitude()));
}

char Field::judgeCrash(short num1 , short num2)
{
	char state = '*';
	float distMin = 0;
	bool distMinFirstFlag = true;
	Vector relativeVelocity;

	short lineNum1 , lineNum2;

	if(stateCrash[num1][num2] == '*') return('*');

	relativeVelocity = object[num1]->getVelocity() - object[num2]->getVelocity();

	for(short h = 0  ;  h < 2  ;  h++){
		Object* objectPolygon;
		Object* objectVertex;

		short polygonNum;
		short vertexNum;
		float A[3] , B[3] , V[3] , P[3] , S[3];

		if(stateCrash[num1][num2] == 'c') break;
		if((h == 0  &&  stateCrash[num1][num2] == 'b')  ||  (h == 1  &&  stateCrash[num1][num2] == 'a')){
			relativeVelocity.multiply(-1);
			continue;
		}

		switch(h){
			case 0 : {
				objectPolygon = object[num1];
				objectVertex = object[num2];
				break;
			}
			case 1 : {
				objectPolygon = object[num2];
				objectVertex = object[num1];
				break;
			}
		}

		polygonNum = objectPolygon->getPolygonNum();
		vertexNum = objectVertex->getVertexNum();

		relativeVelocity.getVector(V);

		for(short i = 0  ;  i < polygonNum  ;  i++){
			switch(stateCrash[num1][num2]){
				case 'a' : {//-> object1 : polygon , object2 : vertex
					if(indexCrash[num1][num2] != i) continue;//h = 0
					break;
				}
				case 'b' : {//-> object1 : vertex  , object2 : polygon
					if(indexCrash[num2][num1] != i) continue;//h = 1
					break;
				}
			}
			if(objectPolygon->isPolygonEmbody(i) == false)
				continue;

			(objectPolygon->getPolygon2Vertex(i) - objectPolygon->getPolygon1Vertex(i)).getVector(A);
			(objectPolygon->getPolygon3Vertex(i) - objectPolygon->getPolygon1Vertex(i)).getVector(B);

			for(short j = 0  ;  j < vertexNum  ;  j++){
				switch(stateCrash[num1][num2]){
					case 'a' : {
						if(indexCrash[num2][num1] != j) continue;
						break;
					}
					case 'b' : {
						if(indexCrash[num1][num2] != j) continue;
						break;
					}
				}
				if(objectVertex->isVertexEmbody(j) == false)
					continue;

				(objectVertex->getVertex(j) - objectPolygon->getPolygon1Vertex(i)).getVector(P);

				if(cubicEquation(A , B , V , P , S)){
					if(stateCrash[num1][num2] == '\0'){
						if(0 <= S[0]  &&  0 <= S[1]  &&  S[0] + S[1] <= 1){
							if((S[2] < distMin  ||  distMinFirstFlag == true)  &&  S[2] > 0){
								distMin = S[2];
								distMinFirstFlag = false;
								switch(h){
									case 0 : {
										indexCrash[num1][num2] = i;
										indexCrash[num2][num1] = j;
										state = 'a';
										break;
									}
									case 1 : {
										indexCrash[num1][num2] = j;
										indexCrash[num2][num1] = i;
										state = 'b';
										break;
									}
								}
							}
						}
					}else{
						if(S[2] <= 0){
							switch(h){
								case 0 : return('A');
								case 1 : return('B');
							}
						}else{
							switch(h){
								case 0 : return('a');
								case 1 : return('b');
							}
						}
					}
				}
			}
		}

		relativeVelocity.multiply(-1);
	}

	lineNum1 = object[num1]->getLineNum();
	lineNum2 = object[num2]->getLineNum();

	for(short i = 0  ;  i < lineNum1  ;  i++){
		float V[3] , P[3] , Q[3] , R[3] , S[3];

		if(stateCrash[num1][num2] == 'c'  &&  indexCrash[num1][num2] != i) continue;


		relativeVelocity.getVector(V);
		(object[num1]->getLineLVertex(i) - object[num1]->getLineRVertex(i)).getVector(P);

		for(short j = 0  ;  j < lineNum2  ;  j++){
			if(stateCrash[num1][num2] == 'c'  &&  indexCrash[num2][num1] != j) continue;

			(object[num2]->getLineRVertex(j) - object[num2]->getLineLVertex(j)).getVector(Q);
			(object[num2]->getLineRVertex(j) - object[num1]->getLineRVertex(i)).getVector(R);////////////////////////////

			if(cubicEquation(V , P , Q , R , S)){
				if(stateCrash[num1][num2] == '\0'){
					if((0 <= S[1]  &&  S[1] <= 1)  &&  (0 <= S[2]  &&  S[2] <= 1)){
						if((S[0] < distMin  ||  distMinFirstFlag == true)  &&  S[0] > 0){
							distMin = S[0];
							distMinFirstFlag = false;

							indexCrash[num1][num2] = i;
							indexCrash[num2][num1] = j;
							state = 'c';
						}
					}
				}else{
					if(S[0] <= 0)
						return('C');
					else
						return('c');
				}
			}
		}
	}

	return(state);
}

void Field::reflect(short formerNum , short latterNum)
{
	float V[3] , P[3] , Q[3] , n[3] , coefficient[3];

	switch(stateCrash[formerNum][latterNum]){
		case 'A' : {
			(object[formerNum]->getPolygon2Vertex(indexCrash[formerNum][latterNum]) - object[formerNum]->getPolygon1Vertex(indexCrash[formerNum][latterNum])).getVector(P);
			(object[formerNum]->getPolygon3Vertex(indexCrash[formerNum][latterNum]) - object[formerNum]->getPolygon1Vertex(indexCrash[formerNum][latterNum])).getVector(Q);

			break;
		}
		case 'B' : {
			(object[latterNum]->getPolygon2Vertex(indexCrash[latterNum][formerNum]) - object[latterNum]->getPolygon1Vertex(indexCrash[latterNum][formerNum])).getVector(P);
			(object[latterNum]->getPolygon3Vertex(indexCrash[latterNum][formerNum]) - object[latterNum]->getPolygon1Vertex(indexCrash[latterNum][formerNum])).getVector(Q);

			break;
		}
		case 'C' : {
			(object[formerNum]->getLineLVertex(indexCrash[formerNum][latterNum]) - object[formerNum]->getLineRVertex(indexCrash[formerNum][latterNum])).getVector(P);
			(object[latterNum]->getLineLVertex(indexCrash[latterNum][formerNum]) - object[latterNum]->getLineRVertex(indexCrash[latterNum][formerNum])).getVector(Q);

			break;
		}
	}

	if(calculate1(P , Q , n) == false){
		if(calculate1(Q , P , n) == false){
			n[0] = 2;
			n[1] = 2;
			n[2] = 2;

			if(P[0] == 0  &&  P[1] == 0)
				n[2] = 0;
			if(P[0] == 0  &&  P[2] == 0)
				n[1] = 0;
			if(P[1] == 0  &&  P[2] == 0)
				n[0] = 0;

			if(Q[0] == 0  &&  Q[1] == 0)
				n[2] = 0;
			if(Q[0] == 0  &&  Q[2] == 0)
				n[1] = 0;
			if(Q[1] == 0  &&  Q[2] == 0)
				n[0] = 0;

			if(n[0] == 0  &&  n[1] == 0)
				n[2] = 1;
			if(n[0] == 0  &&  n[2] == 0)
				n[1] = 1;
			if(n[1] == 0  &&  n[2] == 0)
				n[0] = 1;

			if(n[0] + n[1] + n[2] >= 1.9){
				std::cerr << "reflection failed...   code:" << n[0] + n[1] + n[2] << '\n';
				return;
			}else{
//				std::cerr << "reflection one chance\n";
			}
		}
	}

	for(short i = 0  ;  i < 2  ;  i++){
		short index;
		Vector temp;

		switch(i){
			case 0 : index = formerNum;	break;
			case 1 : index = latterNum;	break;
		}

		object[index]->getVelocity(&temp);
		temp.getVector(V);

		if(cubicEquation(P , Q , n , V , coefficient)){
			V[0] -= 2.0 * coefficient[2] * n[0];
			V[1] -= 2.0 * coefficient[2] * n[1];
			V[2] -= 2.0 * coefficient[2] * n[2];

			temp.setVector(V);
			object[index]->setVelocity(&temp);
		}
	}
}

bool Field::calculate1(float* a , float* b , float* ans)
{
	float temp1, temp2;
	short s = 0;
	short t = 1;
	short u = 2;

	for(short i = 0  ;  i < 6  ;  i++){
		switch(i){
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
		if(a[s] == 0  ||  a[t] == 0  ||  (b[s] - a[s]/a[t]*b[t]) == 0){
//			std::cerr << "ERROR : reflect     ";
			if(i == 5){
//				std::cerr << "sad...\n";
				return(false);
			}
		}else{
			break;
		}
	}

	temp1 = (a[u]/a[t]*b[t] - b[u]) / (b[s] - a[s]/a[t]*b[t]);
	temp2 = (a[u]/a[s]*b[s] - b[u]) / (b[t] - a[t]/a[s]*b[s]);

	ans[u] = sqrt(1 / (temp1*temp1 + temp2*temp2 + 1));//z
	ans[s] = temp1 * ans[2];//x
	ans[t] = temp2 * ans[2];//y

	return(true);
}

bool Field::cubicEquation(float* a , float* b , float* c , float* k , float* ans)
{
	float det = a[0]*b[1]*c[2] + a[1]*b[2]*c[0] + a[2]*b[0]*c[1] - a[2]*b[1]*c[0] - a[1]*b[0]*c[2] - a[0]*b[2]*c[1];

	if(det == 0){
//		std::cerr << "ERROR : det = 0\n";
		return(false);
	}

	ans[0] = ((b[1]*c[2] - b[2]*c[1])*k[0] + (b[2]*c[0] - b[0]*c[2])*k[1] + (b[0]*c[1] - b[1]*c[0])*k[2]) / det;
	ans[1] = ((c[1]*a[2] - c[2]*a[1])*k[0] + (c[2]*a[0] - c[0]*a[2])*k[1] + (c[0]*a[1] - c[1]*a[0])*k[2]) / det;
	ans[2] = ((a[1]*b[2] - a[2]*b[1])*k[0] + (a[2]*b[0] - a[0]*b[2])*k[1] + (a[0]*b[1] - a[1]*b[0])*k[2]) / det;

	return(true);
}
