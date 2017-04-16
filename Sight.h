#ifndef SIGHT_H
#define SIGHT_H

#include <GL/glu.h>
#include <QGLWidget>
#include "Vector.h"

#define SPEED 0.15
#define DOMAIN_MAX 29.85
#define OMEGA 0.8


class Object;
class QKeyEvent;

class Sight : public QGLWidget
{
	Q_OBJECT
public:
	Sight(Object** , short , short);
	void update(void);
	void updateObject(Object** , short);
	void receiveMovement(void);
	short getDominatorIndex(void);
signals:
	void timeCall(void);
private:
	void initializeGL(void);
	void resizeGL(int , int);
	void paintGL(void);
	void paintObject(Object*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
	void rotateSelf(Vector* , Vector* , float , float);

	short objectNum;
	float X , Y , Z;
	float omegaYaw;
	float omegaPitch;
	short dominatorIndex;
	short possessFlag;
	Object** object;
	Vector lookAt;
	Vector lookAtN;
	Vector velocity;
	Vector* dominatorSightPoint;

	short gbFlag;
};

#endif
