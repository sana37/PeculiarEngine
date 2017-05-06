#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include "Array.h"

#define OBJECT_NUM	7
#define EVENT_NUM	4
#define TIME_UNIT	10
#define WORLD		80
#define GRAVITY		0.004
#define NEAR_ZERO	0.03

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
	class SightMoveEvent;
	class CrashEvent;
	class ForceEvent;

private:
//singleton object field
	static Field* field;

	Field(void);
	Field(const Field&);
	virtual ~Field(void);

public:
	static Field* getInstance(void);
	static void deleteInstance(void);
	void open(void);

signals:
	void reportScore(short);

public slots:
	void execTimeEvent(void);
	void timeControl(void);
	void autoGenerate(void);

private:
	void addObject(Object*);
	void addForce(Force*);
	void finishForce(short);

	Sight* sight;
	QTimer* time;
	QTimer* autoGeneration;
//	Object** object;
	Array<Object*> object;
	Array<Force*> force;
	Array<Event*> event;

//	short objectNum;

	short deadObjectIndex[100];
	short deadObjectNum;
	short autoGenerationIndex;
};

#endif
