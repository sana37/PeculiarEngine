#include "Field.h"

#include "Sight.h"
#include "Force.h"

#include "Object.h"
#include "Player.h"
#include "NumberBox.h"
#include "Gunner.h"

#include "MoveEvent.h"
#include "SightMoveEvent.h"
#include "CrashEvent.h"
#include "ForceEvent.h"

#include <QTimer>
#include <iostream>

Field::Field(void)
{
	std::cerr << "Hello world\n";
	objectNum = OBJECT_NUM;/////////
	eventNum = EVENT_NUM;
	forceNum = 0;

	time = new QTimer;
	autoGeneration = new QTimer;

	object = new Object*[objectNum];
	object[0] = new Object("ground0");
	object[1] = new Object("sky1");
	object[2] = new Gunner("player", "bullet");

	object[3] = new Object("object0");
	object[4] = new Object("object1");
	object[5] = new Object("object2");
	object[6] = new Object("object0");
/*
	object[3] = new NumberBox(54);
	object[4] = new NumberBox(50);
	object[5] = new NumberBox(91);
	object[6] = new NumberBox(72);
*/
	sight = new Sight(object, objectNum, 2);

	event = new Event*[eventNum];
	event[0] = new ForceEvent();
	event[1] = new MoveEvent();
	event[2] = new SightMoveEvent();
	event[3] = new CrashEvent();

	force = NULL;
//
	for (short i = 3  ;  i < 7  ;  i++) {
		Force* force = new Force(
			Vector(0, -GRAVITY, 0) * object[i]->getMass(),
			object[i]->getGravityCenter(),
			object[i],
			NULL
		);
		force->setPermanent(true);
		addForce(force);
	}
//
	time->start(TIME_UNIT);
	autoGeneration->start(5000);
	connect(time, SIGNAL(timeout()), this, SLOT(execTimeEvent()));
	QObject::connect(sight, SIGNAL(timeCall()), this, SLOT(timeControl()));
//	QObject::connect(autoGeneration, SIGNAL(timeout()), this, SLOT(autoGenerate()));

	object[3]->moveAbsolute(8, 12, 0);
//	object[3]->setVelocity(0.03, 0.05, 0);
	object[3]->setVelocity(0, 0, 0);
	object[3]->setOmega(1, 1, 0);
	for (short i = 0  ;  i < 10  ;  i++)
		object[3]->rotate();
	object[3]->setOmega(0, 0, 0);

	object[4]->moveAbsolute(6, 16, 6);
//	object[4]->setVelocity(-0.03, 0.03, 0.08);
	object[4]->setVelocity(0, 0, 0);

	object[5]->moveAbsolute(3, 13, 3);
//	object[5]->setVelocity(0, -0.05, 0.03);
	object[5]->setVelocity(0, 0, 0);
	object[5]->setOmega(0.1, 0.1, 0);
	object[5]->rotate();
	object[5]->setOmega(0, 0, 0);

	object[6]->moveAbsolute(0, 15, 0);
//	object[6]->setVelocity(0.04, 0.1, 0);
	object[6]->setVelocity(0, 0, 0);

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
	sight->resize(1000, 700);
	sight->move(400, 0);
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
//event[2] is always enabled
	if (event[0]->isEnabled()) {
		event[0]->disable();
		event[1]->disable();
		event[3]->disable();
	} else {
		event[0]->enable();
		event[1]->enable();
		event[3]->enable();
	}
}

void Field::autoGenerate(void)
{
	Object* temp;
	Vector vector;

	switch (autoGenerationIndex) {//atode random ni suru
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

	switch (autoGenerationIndex % 4) {
		case 0 : {
			vector.setVector(25, 45, 25);
			break;
		}
		case 1 : {
			vector.setVector(25, 45, -25);
			break;
		}
		case 2 : {
			vector.setVector(-25, 45, -25);
			break;
		}
		case 3 : {
			vector.setVector(-25, 45, 25);
			break;
		}
	}
	temp->moveAbsolute(vector);

	switch (autoGenerationIndex % 3) {
		case 0 : {
			vector.setVector(0.02, 0.04, 0);
			break;
		}
		case 1 : {
			vector.setVector(-0.01, 0.01, 0.04);
			break;
		}
		case 2 : {
			vector.setVector(0, -0.04, 0.02);
			break;
		}
	}
	temp->setVelocity(vector);

	objectGenerate(temp);
	autoGenerationIndex++;
}

void Field::objectGenerate(Object* newObject)
{
/*
	if (deadObjectNum > 0) {
		deadObjectNum--;
		delete object[deadObjectIndex[deadObjectNum]];
//		object[deadObjectIndex[deadObjectNum]] = newObject;
		switch (newObject->whichClass()) {
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
	Object** tempObject = new Object*[objectNum];

	for (short i = 0  ;  i < objectNum  ;  i++) {
		if (i == objectNum - 1) {
			switch (newObject->whichClass()) {
				case 'O' : tempObject[i] = new Object(*newObject);	break;
				case 'P' : tempObject[i] = new Player(*(dynamic_cast<Player*>(newObject)));	break;
				case 'G' : tempObject[i] = new Gunner(*(dynamic_cast<Gunner*>(newObject)));	break;
				case 'N' : tempObject[i] = new NumberBox(*(dynamic_cast<NumberBox*>(newObject)));	break;
			}
		} else {
			tempObject[i] = object[i];
		}
	}

	delete[] object;

	object = tempObject;

	sight->updateObject(object, objectNum);
}

void Field::addForce(Force* force)
{
	forceNum++;
	Force** tempForce = new Force*[forceNum];

	for (short i = 0  ;  i < forceNum  ;  i++) {
		if (i != forceNum - 1) {
			tempForce[i] = this->force[i];
		} else {
			tempForce[i] = force;
		}
	}

	if (this->force != NULL)
		delete[] (this->force);

	this->force = tempForce;
	sight->updateForce(this->force, forceNum);
}

void Field::finishForce(short idx)
{
	delete force[idx];

	forceNum--;
	Force** tempForce = new Force*[forceNum];

	for (short i = 0  ;  i < forceNum  ;  i++) {
		if (i < idx) {
			tempForce[i] = force[i];
		} else {
			tempForce[i] = force[i + 1];
		}
	}

	delete[] force;
	force = tempForce;
	sight->updateForce(this->force, forceNum);
}

Field* Field::field = NULL;
