#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include "Array.h"

class Sight;
class Object;
class Force;
class QTimer;

class Field : public QObject
{
	Q_OBJECT
public:
	class Event;
	class MoveEvent;
	class CrashEvent;
	class ForceEvent;

private:
	static Field* field;

	Field(void);
	Field(const Field&);
	virtual ~Field(void);

public:
	static Field* getInstance(void);
	static void deleteInstance(void);

	void open(void);

	Object* getObject(int);
	int getObjectNum(void);
	void addObject(Object*);
	void deleteObject(Object*);
	void addForce(Force*);
	void deleteForce(Object*);

public slots:
	void execTimeEvent(void);
	void timeControl(void);

private:
	Sight* sight;
	QTimer* time;
	Array<Object*> object;
	Array<Force*> force;
	Event* forceEvent;
	Event* crashEvent;
	Event* moveEvent;

};

#endif
