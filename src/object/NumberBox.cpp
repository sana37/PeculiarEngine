#include "NumberBox.h"
#include <math.h>
#include <iostream>//

NumberBox::NumberBox(short num) : Object::Object("res/cube")
{
//	classCode = 'N';
	number = num;
	this->setup(number);
}
//this copy constructer is wrong!! vertexs increase permanently
/*
NumberBox::NumberBox(const NumberBox& originalNumberBox) : Object::Object(originalNumberBox)
{
	Vector temp = this->getGravityCenter();

	this->moveAbsolute(0, 0, 0);

	number = originalNumberBox.getNum();
	this->setup(number);

	this->moveAbsolute(temp);
}

NumberBox::NumberBox(const NumberBox& numberBox, short num) : Object::Object(dynamic_cast<const Object&>(numberBox))
{
	Vector temp 
}
*/
void NumberBox::setup(short num)
{
	Object* temp;

	switch (num / 10) {
		case 0 : {
			temp = new Object("res/l0");
			break;
		}
		case 1 : {
			temp = new Object("res/l1");
			break;
		}
		case 2 : {
			temp = new Object("res/l2");
			break;
		}
		case 3 : {
			temp = new Object("res/l3");
			break;
		}
		case 4 : {
			temp = new Object("res/l4");
			break;
		}
		case 5 : {
			temp = new Object("res/l5");
			break;
		}
		case 6 : {
			temp = new Object("res/l6");
			break;
		}
		case 7 : {
			temp = new Object("res/l7");
			break;
		}
		case 8 : {
			temp = new Object("res/l8");
			break;
		}
		case 9 : {
			temp = new Object("res/l9");
			break;
		}
		default: {
			temp = new Object("res/l0");
			break;
		}
	}
	this->composeObject(temp);
	delete temp;

	switch (num % 10) {
		case 0 : {
			temp = new Object("res/r0");
			break;
		}
		case 1 : {
			temp = new Object("res/r1");
			break;
		}
		case 2 : {
			temp = new Object("res/r2");
			break;
		}
		case 3 : {
			temp = new Object("res/r3");
			break;
		}
		case 4 : {
			temp = new Object("res/r4");
			break;
		}
		case 5 : {
			temp = new Object("res/r5");
			break;
		}
		case 6 : {
			temp = new Object("res/r6");
			break;
		}
		case 7 : {
			temp = new Object("res/r7");
			break;
		}
		case 8 : {
			temp = new Object("res/r8");
			break;
		}
		case 9 : {
			temp = new Object("res/r9");
			break;
		}
		default: {
			temp = new Object("res/r0");
			break;
		}
	}
	this->composeObject(temp);
	delete temp;
}

short NumberBox::getNum(void) const
{
	return number;
}

bool NumberBox::decompose(NumberBox** operatedNumberBox, Object** operatedObject)
{
	Vector replica1Velocity = (*operatedNumberBox)->getVelocity();
	Vector replica2Velocity = (*operatedObject)->getVelocity() * 0.01;
	Vector replica1Point = (*operatedNumberBox)->getGravityCenter();
	Vector replica2Point = (*operatedObject)->getGravityCenter() + (replica2Velocity * (3.0 / replica2Velocity.getMagnitude()));//magnitude = 0??
//	omega copy
	short replicaNumber1 = (*operatedNumberBox)->getNum();
	short replicaNumber2 = sqrt((*operatedNumberBox)->getNum());

	for (  ;  replicaNumber2 > 0  ;  replicaNumber2--) {
		if (replicaNumber1 % replicaNumber2 == 0) {
			replicaNumber1 /= replicaNumber2;
			break;
		}
	}
	std::cerr << "decompose(new)\n";
	NumberBox* replica1 = new NumberBox(replicaNumber1);
	NumberBox* replica2 = new NumberBox(replicaNumber2);

	replica1->moveAbsolute(replica1Point);
	replica2->moveAbsolute(replica2Point);
	replica1->setVelocity(replica1Velocity);
	replica2->setVelocity(replica2Velocity);

	delete *operatedNumberBox;
	delete *operatedObject;

	*operatedNumberBox = replica1;
	*operatedObject = replica2;

	std::cerr << "decompose end\n";
	if (replicaNumber1 == 1  ||  replicaNumber2 == 1) {
		return false;
	} else {
		return true;
	}
}

char NumberBox::whichClass(void)
{
	return 'N';
}
