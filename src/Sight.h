#ifndef SIGHT_H
#define SIGHT_H

#include <GL/glu.h>
#include <QGLWidget>
#include "Vector.h"


class Object;
class PlayerNeo;
class UniversalForce;
class UniversalTorque;
class QKeyEvent;

class Sight : public QGLWidget
{
	Q_OBJECT
public:
	Sight(PlayerNeo*, UniversalForce*, UniversalTorque*);

//	void update(void);
signals:
	void timeCall(void);
private:
	void initializeGL(void);
	void resizeGL(int, int);
	void paintGL(void);
	void setGluLookAt(const Vector&, const Vector&);
	void paintObject(Object*);
//	void paintCrashSpot(void);
	void keyPressEvent(QKeyEvent*);
	void keyReleaseEvent(QKeyEvent*);
//	void rotateSelf(Vector*, Vector, float, float);

	float omegaYaw;
	float omegaPitch;
	PlayerNeo* playerNeo;
	UniversalForce* accel;
	UniversalTorque* torque;

};

#endif
