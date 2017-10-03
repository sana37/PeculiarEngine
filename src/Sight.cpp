#include "Sight.h"
#include "Field.h"
#include "Object.h"
#include "Gunner.h"
#include "PlayerNeo.h"
#include "Calculater.h"
#include "UniversalForce.h"
#include "UniversalTorque.h"
#include "Define.h"
#include <QKeyEvent>
#include <iostream>


Sight::Sight(PlayerNeo* playerNeo, UniversalForce* accel, UniversalTorque* torque)
{
	omegaYaw = 0;
	omegaPitch = 0;

	this->playerNeo = playerNeo;
	this->accel = accel;
	this->torque = torque;

	updateGL();
}
/*
void Sight::update(void)
{
	Vector zero(0, 0, 0);

	if ((-DOMAIN_MAX < X  &&  velocity.getX() < 0)  ||  (X < DOMAIN_MAX  &&  velocity.getX() > 0))
		X += velocity.getX();
	if ((FLOOR < Y  &&  velocity.getY() < 0)  ||  (Y < CEILING  &&  velocity.getY() > 0))
		Y += velocity.getY();
	if ((-DOMAIN_MAX < Z  &&  velocity.getZ() < 0)  ||  (Z < DOMAIN_MAX  &&  velocity.getZ() > 0))
		Z += velocity.getZ();


	if (lookAt.getY() > 0.97  &&  omegaPitch > 0)
		omegaPitch = 0;
	if (lookAt.getY() < -0.97  &&  omegaPitch < 0)
		omegaPitch = 0;

	rotateSelf(&lookAt, zero, omegaYaw, omegaPitch);
	rotateSelf(&lookAtN, zero, omegaYaw, 0);

	if (omegaYaw != 0  ||  omegaPitch != 0) {
		if ((*object_p)[dominatorIndex]->whichClass() == 'G') {
			Gunner* gunner = (Gunner*) (*object_p)[dominatorIndex];

			Vector omega(0, omegaYaw, 0);
			gunner->rotateBullet(omega);
			omega = lookAtN * omegaPitch;
			gunner->rotateBullet(omega);
		}

		if (gbFlag != 0)
			velocity = lookAt * SPEED * gbFlag;
	}
}
*/
void Sight::initializeGL(void)
{
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
//	glOrtho(-5, 5, -5, 5, -10, 10);
//	glFrustum(-1, 1, -1, 1, 1, 15);
//	gluPerspective(90, 1, 0.01, 100);
	gluPerspective(60, 1, 0.01, 100);
}

void Sight::resizeGL(int width, int height)
{
	if (width > height)
		glViewport((width - height) / 2.0, 0, height, height);
	else
		glViewport(0, (height - width) / 2.0, width, width);
}

void Sight::paintGL(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector sightPoint = this->playerNeo->getSightPointAbsolute();
	Vector lookAt = this->playerNeo->getLookAt();
//	Vector lookAtN = this->playerNeo->getLookAtN();
	float X = sightPoint.getX();
	float Y = sightPoint.getY();
	float Z = sightPoint.getZ();

	gluLookAt(X, Y, Z, X + lookAt.getX(), Y + lookAt.getY(), Z + lookAt.getZ(), 0, 1, 0);

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	Field* field = Field::getInstance();
	int objectNum = field->getObjectNum();

	for (int i = 0; i < objectNum; i++) {
		paintObject(field->getObject(i));
	}
//	paintCrashSpot();

	glBegin(GL_LINES);
/*
	glColor3d(0.8, 0, 0);
	glVertex3d(X + lookAt.getX() + (lookAtN.getX() / 10), Y + lookAt.getY() + (lookAtN.getY() / 10), Z + lookAt.getZ() + (lookAtN.getZ() / 10));
	glVertex3d(X + lookAt.getX() + (lookAtN.getX() / 50), Y + lookAt.getY() + (lookAtN.getY() / 50), Z + lookAt.getZ() + (lookAtN.getZ() / 50));
	glVertex3d(X + lookAt.getX() - (lookAtN.getX() / 10), Y + lookAt.getY() - (lookAtN.getY() / 10), Z + lookAt.getZ() - (lookAtN.getZ() / 10));
	glVertex3d(X + lookAt.getX() - (lookAtN.getX() / 50), Y + lookAt.getY() - (lookAtN.getY() / 50), Z + lookAt.getZ() - (lookAtN.getZ() / 50));
*/
	glColor3d(0, 0, 0);
	glVertex3d( BAR_POS,  0,  BAR_POS);
	glVertex3d( BAR_POS, 2 * BAR_POS,  BAR_POS);
	glVertex3d( BAR_POS,  0, -BAR_POS);
	glVertex3d( BAR_POS, 2 * BAR_POS, -BAR_POS);
	glVertex3d(-BAR_POS,  0,  BAR_POS);
	glVertex3d(-BAR_POS, 2 * BAR_POS,  BAR_POS);
	glVertex3d(-BAR_POS,  0, -BAR_POS);
	glVertex3d(-BAR_POS, 2 * BAR_POS, -BAR_POS);
	glEnd();

}

void Sight::paintObject(Object* modelObject)
{
	short num = modelObject->getPolygonNum();

	glBegin(GL_TRIANGLES);//time loss?
	for (short i = 0; i < num; i++) {
		Vector temp;

		glColor3s(modelObject->getPolygonR(i), modelObject->getPolygonG(i), modelObject->getPolygonB(i));

		temp = modelObject->getPolygon1Vertex(i);
		glVertex3d(temp.getX(), temp.getY(), temp.getZ());
		temp = modelObject->getPolygon2Vertex(i);
		glVertex3d(temp.getX(), temp.getY(), temp.getZ());
		temp = modelObject->getPolygon3Vertex(i);
		glVertex3d(temp.getX(), temp.getY(), temp.getZ());
	}
	glEnd();
/*
	glColor3s(0, 0, 0);
	glBegin(GL_LINES);
	for (short i = 0; i < num; i++) {
		Vector p1 = (modelObject->getPolygon1Vertex(i) + modelObject->getPolygon2Vertex(i) + modelObject->getPolygon3Vertex(i)) / 3.0;
		Vector p2 = p1 - (modelObject->getPlgnInside(i));

		glVertex3d(p1.getX(), p1.getY(), p1.getZ());
		glVertex3d(p2.getX(), p2.getY(), p2.getZ());
	}
	glEnd();
*/
}
/*
void Sight::paintCrashSpot(void)
{
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3s(32767, 32767, 32767);

	for (short i = 0  ;  i < force_p->length()  ;  i++) {
		Vector crashSpot = force_p->get(i)->getForcePoint();
		glVertex3d(crashSpot.getX(), crashSpot.getY(), crashSpot.getZ());
	}

	glEnd();
}
*/
void Sight::keyPressEvent(QKeyEvent* keyboard)
{
	int ch;

	if (keyboard->isAutoRepeat()) {
//		std::cerr << "miss press\n";
		return;
	}

	ch = keyboard->key();

	switch (ch) {
		case 16777234 : {//turn left
			torque->setVector(0, TORQUE, 0);
//			should rotate the direction of accel?
//			omegaYaw = OMEGA;
			break;
		}
		case 16777236 : {//turn right
			torque->setVector(0, -TORQUE, 0);
//			omegaYaw = -OMEGA;
			break;
		}
/*
		case 16777235 : {//look up
//			torque->setVector(lookAtN * TORQUE);
//			omegaPitch = OMEGA;
			break;
		}
		case 16777237 : {//look down
//			torque->setVector(lookAtN * -TORQUE);
//			omegaPitch = -OMEGA;
			break;
		}
		case ' ' : {
			if ((*object_p)[dominatorIndex]->whichClass() == 'G') {
				Gunner* gunner = dynamic_cast<Gunner*>((*object_p)[dominatorIndex]);
				gunner->trigger(lookAt);
			}
			break;
		}
*/

		case 'W' : {//go
			accel->setVector(this->playerNeo->getLookAt() * ACCEL);
			break;
		}
		case 'S' : {//back
			accel->setVector(this->playerNeo->getLookAt() * -ACCEL);
			break;
		}

/*
		case 'I' : {//up
			accel->setVector(0, ACCEL, 0);
			break;
		}
		case 'K' : {
			accel->setVector(0, -ACCEL, 0);
			break;
		}
*/
		case 16777220 : {
			timeCall();
			break;
		}

	}
}

void Sight::keyReleaseEvent(QKeyEvent* keyboard)
{
	int ch = keyboard->key();

	if (keyboard->isAutoRepeat()) {
//		std::cerr << "miss release\n";
		return;
	}

	switch (ch) {
		case 16777234 ://turn left
		case 16777236 : {//turn right
			torque->setVector(0, 0, 0);
			this->playerNeo->stop();
//			omegaYaw = 0;
			break;
		}
/*
		case 16777235 ://look up
		case 16777237 : {//look down
			torque->setVector(0, 0, 0);
			this->playerNeo->stop();
//			omegaPitch = 0;
			break;
		}

		case 'I' ://up
		case 'K' ://down
*/
		case 'W' ://go
		case 'S' : {//back
			accel->setVector(0, 0, 0);
			this->playerNeo->stop();
			break;
		}

	}
}
/*
void Sight::rotateSelf(Vector* vertex_p, Vector basePoint, float degYaw, float degPitch)
{
	Vector shaft(0, 1, 0);

	Calculater::rotateRad(vertex_p, basePoint, shaft, degYaw);
	Calculater::rotateRad(vertex_p, basePoint, lookAtN, degPitch);
}
*/