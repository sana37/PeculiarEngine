#include "Sight.h"
#include "Object.h"
#include "Gunner.h"
#include "Calculater.h"
#include <QKeyEvent>
#include <iostream>


Sight::Sight(Object** originalObject , short originalObjectNum , short originalDominatorIndex) :
	lookAt(Vector(0 , 0 , -1)) ,
	lookAtN(Vector(1 , 0 , 0)) ,
	velocity(Vector(0 , 0 , 0))
{
	gbFlag = 0;
	Vector temp(0 , 30 , 25);

	X = temp.getX();
	Y = temp.getY();
	Z = temp.getZ();

	omegaYaw = 0;
	omegaPitch = 0;

	objectNum = originalObjectNum;
	dominatorIndex = originalDominatorIndex;

	object = originalObject;
	dominatorSightPoint = new Vector[objectNum];
	object[dominatorIndex]->moveAbsolute(temp);
//////////////////////////////////////////////////////////////////////////////// koudaisai only
	object[dominatorIndex]->stop();
	dominatorSightPoint[originalDominatorIndex].setVector(0 , 0.1 , -0.1);

	possessFlag = 2;
	object[dominatorIndex]->setDomination(true);
	std::cerr << "possessing\n";
//////////////////////////////////////////////////////////////////////////////// koudaisai only
	updateGL();
}

void Sight::update(void)
{
	Vector zero(0 , 0 , 0);

	if((-DOMAIN_MAX < X  &&  velocity.getX() < 0)  ||  (X < DOMAIN_MAX  &&  velocity.getX() > 0))
		X += velocity.getX();
	if((0.15 < Y  &&  velocity.getY() < 0)  ||  (Y < 2 * DOMAIN_MAX  &&  velocity.getY() > 0))
		Y += velocity.getY();
	if((-DOMAIN_MAX < Z  &&  velocity.getZ() < 0)  ||  (Z < DOMAIN_MAX  &&  velocity.getZ() > 0))
		Z += velocity.getZ();

	if(possessFlag == 1){
		dominatorSightPoint[dominatorIndex] += velocity;
	}

	if(lookAt.getY() > 0.97  &&  omegaPitch > 0)
		omegaPitch = 0;
	if(lookAt.getY() < -0.97  &&  omegaPitch < 0)
		omegaPitch = 0;

	rotateSelf(&lookAt , &zero , omegaYaw , omegaPitch);
	rotateSelf(&lookAtN , &zero , omegaYaw , 0);

	if(possessFlag == 2){
		Vector basePoint(X , Y , Z);
		Vector vertex;

		vertex = basePoint - dominatorSightPoint[dominatorIndex];
		object[dominatorIndex]->moveAbsolute(vertex);

		if(omegaYaw != 0  ||  omegaPitch != 0){
			short vertexNum;

			vertexNum = object[dominatorIndex]->getVertexNum();
			for(short i = 0  ;  i < vertexNum  ;  i++){
				vertex = object[dominatorIndex]->getVertex(i);
				rotateSelf(&vertex , &basePoint , omegaYaw , omegaPitch);
				object[dominatorIndex]->setVertex(i , vertex);
			}
			vertex = object[dominatorIndex]->getGravityCenter();
			rotateSelf(&vertex , &basePoint , omegaYaw , omegaPitch);
			object[dominatorIndex]->setGravityCenter(vertex);

			rotateSelf(&dominatorSightPoint[dominatorIndex] , &zero , omegaYaw , omegaPitch);
			if(gbFlag != 0)
				velocity = lookAt * SPEED * gbFlag;
		}
	}
}

void Sight::updateObject(Object** originalObject , short originalObjectNum)
{
	short preObjectNum = objectNum;

	object = originalObject;
	objectNum = originalObjectNum;

	Vector* temp = new Vector[objectNum];

	for(short i = 0  ;  i < objectNum  ;  i++){
		if(i < preObjectNum)
			temp[i].setVector(&dominatorSightPoint[i]);
		else
			temp[i].setVector(0 , 0 , 0);
	}

	delete[] dominatorSightPoint;
	dominatorSightPoint = temp;
}

void Sight::receiveMovement(void)
{
	if(possessFlag > 0){
		Vector temp = object[dominatorIndex]->getVelocity();

		X += temp.getX();
		Y += temp.getY();
		Z += temp.getZ();
	}
}

short Sight::getDominatorIndex(void)
{
	return(dominatorIndex);
}

void Sight::initializeGL(void)
{
	glClearColor(1 , 1 , 1 , 1);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
//	glOrtho(-5 , 5 , -5 , 5 , -10 , 10);
//	glFrustum(-1 , 1 , -1 , 1 , 1 , 15);
//	gluPerspective(90 , 1 , 0.01 , 100);
	gluPerspective(60 , 1 , 0.01 , 100);
}

void Sight::resizeGL(int width , int height)
{
	if(width > height)
		glViewport((width - height) / 2.0 , 0 , height , height);
	else
		glViewport(0 , (height - width) / 2.0 , width , width);
}

void Sight::paintGL(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(X , Y , Z , X + lookAt.getX() , Y + lookAt.getY() , Z + lookAt.getZ() , 0 , 1 , 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	for(short i = 0  ;  i < objectNum  ;  i++){
		paintObject(object[i]);
	}

	glBegin(GL_LINES);
	glColor3d(0.8 , 0 , 0);
	glVertex3d(X + lookAt.getX() + (lookAtN.getX() / 10) , Y + lookAt.getY() + (lookAtN.getY() / 10) , Z + lookAt.getZ() + (lookAtN.getZ() / 10));
	glVertex3d(X + lookAt.getX() + (lookAtN.getX() / 50) , Y + lookAt.getY() + (lookAtN.getY() / 50) , Z + lookAt.getZ() + (lookAtN.getZ() / 50));
	glVertex3d(X + lookAt.getX() - (lookAtN.getX() / 10) , Y + lookAt.getY() - (lookAtN.getY() / 10) , Z + lookAt.getZ() - (lookAtN.getZ() / 10));
	glVertex3d(X + lookAt.getX() - (lookAtN.getX() / 50) , Y + lookAt.getY() - (lookAtN.getY() / 50) , Z + lookAt.getZ() - (lookAtN.getZ() / 50));

	glColor3d(0 , 0 , 0);
	glVertex3d( DOMAIN_MAX ,  0 ,  DOMAIN_MAX);
	glVertex3d( DOMAIN_MAX , 2 * DOMAIN_MAX ,  DOMAIN_MAX);
	glVertex3d( DOMAIN_MAX ,  0 , -DOMAIN_MAX);
	glVertex3d( DOMAIN_MAX , 2 * DOMAIN_MAX , -DOMAIN_MAX);
	glVertex3d(-DOMAIN_MAX ,  0 ,  DOMAIN_MAX);
	glVertex3d(-DOMAIN_MAX , 2 * DOMAIN_MAX ,  DOMAIN_MAX);
	glVertex3d(-DOMAIN_MAX ,  0 , -DOMAIN_MAX);
	glVertex3d(-DOMAIN_MAX , 2 * DOMAIN_MAX , -DOMAIN_MAX);
	glEnd();
}

void Sight::paintObject(Object* modelObject)
{
	short num = modelObject->getPolygonNum();

	glBegin(GL_TRIANGLES);//time loss?
	for(short i = 0  ;  i < num  ;  i++){
		Vector temp;

		glColor3s(modelObject->getPolygonR(i) , modelObject->getPolygonG(i) , modelObject->getPolygonB(i));

		temp = modelObject->getPolygon1Vertex(i);
		glVertex3d(temp.getX() , temp.getY() , temp.getZ());
		temp = modelObject->getPolygon2Vertex(i);
		glVertex3d(temp.getX() , temp.getY() , temp.getZ());
		temp = modelObject->getPolygon3Vertex(i);
		glVertex3d(temp.getX() , temp.getY() , temp.getZ());
	}
	glEnd();
}

void Sight::keyPressEvent(QKeyEvent* keyboard)
{
	int ch;

	if(keyboard->isAutoRepeat()){
//		std::cerr << "miss press\n";
		return;
	}

	ch = keyboard->key();

	switch(ch){
		case 16777234 : {//turn left
			omegaYaw = OMEGA;
			break;
		}
		case 16777236 : {//turn right
			omegaYaw = -OMEGA;
			break;
		}
		case 16777235 : {//look up
			omegaPitch = OMEGA;
			break;
		}
		case 16777237 : {//look down
			omegaPitch = -OMEGA;
			break;
		}
		case ' ' : {
			if(possessFlag == 2  &&  object[dominatorIndex]->whichClass() == 'G'){
				Gunner* gunner = (Gunner*)object[dominatorIndex];
				gunner->trigger(lookAt);
			}
			break;
		}

		case 'W' : {//go
			velocity = lookAt * SPEED;
			gbFlag = 1;
			break;
		}
		case 'S' : {//back
			velocity = lookAt * -SPEED;
			gbFlag = -1;
			break;
		}

/*
		case 'I' : {//up
			velocity.setVector(0 , SPEED , 0);
			break;
		}
		case 'K' : {
			velocity.setVector(0 , -SPEED , 0);
			break;
		}
*/

		case 16777220 : {
			timeCall();
			break;
		}


		case '@' : {
			possessFlag++;
			switch(possessFlag){
				case 1 : {
					Vector temp = object[dominatorIndex]->getGravityCenter() + dominatorSightPoint[dominatorIndex];

					X = temp.getX();
					Y = temp.getY();
					Z = temp.getZ();

					std::cerr << "prepare possess\n";
					break;
				}
				case 2 : {
//					Vector temp(dominatorSightPoint[dominatorIndex]);
					object[dominatorIndex]->setDomination(true);
					std::cerr << "possessing\n";
//					std::cerr << "x : " << temp.getX() << " , y : " << temp.getY() << " , z : " << temp.getZ() << '\n';
					break;
				}
				case 3 : {
					object[dominatorIndex]->setDomination(false);
					possessFlag = 0;
					std::cerr << "release possess\n";
					break;
				}
			}
			break;
		}
		case '[' : {
			if(possessFlag == 1){
				Vector temp;

//				object[dominatorIndex]->setDomination(false);

				dominatorIndex++;
				if(dominatorIndex >= objectNum)
					dominatorIndex = 0;

//				object[dominatorIndex]->setDomination(true);

				temp = object[dominatorIndex]->getGravityCenter() + dominatorSightPoint[dominatorIndex];

				X = temp.getX();
				Y = temp.getY();
				Z = temp.getZ();
			}
			break;
		}
		case ']' : {
			if(possessFlag == 1){
				Vector temp;

//				object[dominatorIndex]->setDomination(false);

				dominatorIndex--;
				if(dominatorIndex < 0)
					dominatorIndex = objectNum - 1;

//				object[dominatorIndex]->setDomination(true);

				temp = object[dominatorIndex]->getGravityCenter() + dominatorSightPoint[dominatorIndex];

				X = temp.getX();
				Y = temp.getY();
				Z = temp.getZ();
			}
			break;
		}

/*
		case 16777220 : {//enter
			break;
		}
*/
	}
}

void Sight::keyReleaseEvent(QKeyEvent* keyboard)
{
	int ch = keyboard->key();

	if(keyboard->isAutoRepeat()){
//		std::cerr << "miss release\n";
		return;
	}

	switch(ch){
		case 16777234 ://turn left
		case 16777236 : {//turn right
			omegaYaw = 0;
			break;
		}
		case 16777235 ://look up
		case 16777237 : {//look down
			omegaPitch = 0;
			break;
		}
/*
		case 'I' ://up
		case 'K' ://down
*/
		case 'W' ://go
		case 'S' : {//back
			velocity.setVector(0 , 0 , 0);
			gbFlag = 0;
			break;
		}

	}
}

void Sight::rotateSelf(Vector* vertex , Vector* basePoint , float degYaw , float degPitch)
{
	Vector shaft(0 , 1 , 0);

	*vertex -= *basePoint;
	Calculater::rotate(vertex , &shaft , degYaw);
	Calculater::rotate(vertex , &lookAtN , degPitch);
	*vertex += *basePoint;
}
