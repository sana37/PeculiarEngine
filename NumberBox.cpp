#include "NumberBox.h"
#include <math.h>
#include <iostream>//

NumberBox::NumberBox(short num) : Object::Object("cube")
{
	classCode = 'N';
	number = num;
	this->setup(number);
}

NumberBox::NumberBox(const NumberBox& originalNumberBox) : Object::Object(originalNumberBox)
{
	Vector temp = this->getGravityCenter();

	this->moveAbsolute(0 , 0 , 0);

	number = originalNumberBox.getNum();
	this->setup(number);

	this->moveAbsolute(temp);
}

void NumberBox::setup(short num)
{
	Object* temp;

	switch(num / 10){
		case 0 : {
			temp = new Object("l0");
			break;
		}
		case 1 : {
			temp = new Object("l1");
			break;
		}
		case 2 : {
			temp = new Object("l2");
			break;
		}
		case 3 : {
			temp = new Object("l3");
			break;
		}
		case 4 : {
			temp = new Object("l4");
			break;
		}
		case 5 : {
			temp = new Object("l5");
			break;
		}
		case 6 : {
			temp = new Object("l6");
			break;
		}
		case 7 : {
			temp = new Object("l7");
			break;
		}
		case 8 : {
			temp = new Object("l8");
			break;
		}
		case 9 : {
			temp = new Object("l9");
			break;
		}
		default: {
			temp = new Object("l0");
			break;
		}
	}
	this->composeObject(temp);
	delete temp;

	switch(num % 10){
		case 0 : {
			temp = new Object("r0");
			break;
		}
		case 1 : {
			temp = new Object("r1");
			break;
		}
		case 2 : {
			temp = new Object("r2");
			break;
		}
		case 3 : {
			temp = new Object("r3");
			break;
		}
		case 4 : {
			temp = new Object("r4");
			break;
		}
		case 5 : {
			temp = new Object("r5");
			break;
		}
		case 6 : {
			temp = new Object("r6");
			break;
		}
		case 7 : {
			temp = new Object("r7");
			break;
		}
		case 8 : {
			temp = new Object("r8");
			break;
		}
		case 9 : {
			temp = new Object("r9");
			break;
		}
		default: {
			temp = new Object("r0");
			break;
		}
	}
	this->composeObject(temp);
	delete temp;
}

short NumberBox::getNum(void) const
{
	return(number);
}

bool NumberBox::decompose(NumberBox** operatedNumberBox , Object** operatedObject)
{
	NumberBox* replica1;
	NumberBox* replica2;
	Vector replica1Velocity = (*operatedNumberBox)->getVelocity();
	Vector replica2Velocity = (*operatedObject)->getVelocity() * 0.01;
	Vector replica1Point = (*operatedNumberBox)->getGravityCenter();
	Vector replica2Point = (*operatedObject)->getGravityCenter() + (replica2Velocity * (3.0 / replica2Velocity.getMagnitude()));
	short replicaNumber1 = (*operatedNumberBox)->getNum();
	short replicaNumber2 = sqrt((*operatedNumberBox)->getNum()) / 1;

	for(  ;  replicaNumber2 > 0  ;  replicaNumber2--){
		if(replicaNumber1 % replicaNumber2 == 0){
			replicaNumber1 /= replicaNumber2;
			break;
		}
	}
	std::cerr << "decompose(new)\n";
	replica1 = new NumberBox(replicaNumber1);
	replica2 = new NumberBox(replicaNumber2);

	replica1->moveAbsolute(replica1Point);
	replica2->moveAbsolute(replica2Point);
	replica1->setVelocity(replica1Velocity);
	replica2->setVelocity(replica2Velocity);

	delete *operatedNumberBox;
	delete *operatedObject;

	*operatedNumberBox = replica1;
	*operatedObject = replica2;

	std::cerr << "decompose end\n";
	if(replicaNumber1 == 1  ||  replicaNumber2 == 1){
		return(false);
	}else{
		return(true);
	}
}
