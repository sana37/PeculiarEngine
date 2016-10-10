#ifndef SIGHT_H
#define SIGHT_H

#include <GL/glu.h>
#include <QGLWidget>

#define PI 3.141592
#define SPEED 0.15

class Object;
class QKeyEvent;
class Vector;

class Sight : public QGLWidget
{
	Q_OBJECT
public:
	Sight(Object** , short , short);
	void update(void);
	void updateObject(Object** , short);
	void receiveMovement(void);
	short getDominatorIndex(void);
public slots:
	void moveSight(void);
signals:
	void timeCall(void);
private:
	void initializeGL(void);
	void resizeGL(int , int);
	void paintGL(void);
	void paintObject(Object*);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
	void rotate(Vector* , Vector* , float , float);
	void spin(Vector* , Vector* , float);
	float matrix1(float , float);
	float matrix2(float , float , float , float);

	short objectNum;
	float X , Y , Z;
	float omegaYaw;
	float omegaPitch;
	short dominatorIndex;
	short possessFlag;
	Object** object;
	Vector* lookAt;
	Vector* lookAtN;
	Vector* dominatorSightPoint;
	Vector* velocity;

	short gbFlag;
};

#endif
