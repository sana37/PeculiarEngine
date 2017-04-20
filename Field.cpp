#include "Field.h"

#include "Sight.h"

#include "Object.h"
#include "Player.h"
#include "NumberBox.h"
#include "Gunner.h"

#include "MoveEvent.h"
#include "SightMoveEvent.h"
#include "CrashEvent.h"

#include <QTimer>
#include <iostream>

Field::Field(void)
{
	std::cerr << "Hello world\n";
	objectNum = OBJECT_NUM;/////////
	eventNum = EVENT_NUM;
	time = new QTimer;
	autoGeneration = new QTimer;
	object = new Object*[objectNum];
	object[0] = new Object("ground0");
	object[1] = new Object("sky1");
	object[2] = new Gunner("player" , "bullet");
/*
	object[3] = new Object("object0");
	object[4] = new Object("object1");
	object[5] = new Object("object2");
	object[6] = new Object("object0");
*/
	object[3] = new NumberBox(54);
	object[4] = new NumberBox(50);
	object[5] = new NumberBox(91);
	object[6] = new NumberBox(72);

	sight = new Sight(object , objectNum , 2);

	event = new Event*[eventNum];
	event[0] = new MoveEvent();
	event[1] = new SightMoveEvent();
	event[2] = new CrashEvent();

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
	autoGeneration->start(5000);
	connect(time , SIGNAL(timeout()) , this , SLOT(execTimeEvent()));
	QObject::connect(sight , SIGNAL(timeCall()) , this , SLOT(timeControl()));
	QObject::connect(autoGeneration , SIGNAL(timeout()) , this , SLOT(autoGenerate()));

	object[3]->moveAbsolute(8 , 2 , 0);
	object[3]->setVelocity(0.03 , 0.05 , 0);
//	object[3]->setVelocity(0 , 0 , 0);

	object[4]->moveAbsolute(6 , 6 , 6);
	object[4]->setVelocity(-0.03 , 0.03 , 0.08);
//	object[4]->setVelocity(0 , 0 , 0);

	object[5]->moveAbsolute(3 , 3 , 3);
	object[5]->setVelocity(0 , -0.05 , 0.03);
//	object[5]->setVelocity(0 , 0 , 0);

	object[6]->moveAbsolute(0 , 5 , 0);
	object[6]->setVelocity(0.04 , 0.1 , 0);
//	object[6]->setVelocity(0 , 0 , 0);

	deadObjectNum = 0;
	autoGenerationIndex = 0;
}
/*
Field::Field(const Field& _field) : QObject::QObject()
{
//do nothing because this class make singleton object
}
*/
Field::~Field(void)
{
	sight->close();
}

Field* Field::getInstance(void)
{
	if (field == NULL)
		field = new Field();
	return(field);
}

void Field::deleteInstance(void)
{
	if (field != NULL)
		delete field;
	field = NULL;
}

void Field::open(void)
{
	sight->resize(1000 , 700);
	sight->move(400 , 0);
//	sight->showMaximized();
	sight->show();
}

void Field::execTimeEvent(void)
{
	for (short i = 0  ;  i < eventNum  ;  i++) {
		if (event[i]->isEnabled())
			event[i]->exec();
	}
	sight->updateGL();
}

void Field::timeControl(void)
{
//event[1] is always enabled
	if (event[0]->isEnabled()) {
		event[0]->disable();
		event[2]->disable();
	} else {
		event[0]->enable();
		event[2]->enable();
	}
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
	temp->moveAbsolute(vector);

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
	temp->setVelocity(vector);

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

Field* Field::field = NULL;
