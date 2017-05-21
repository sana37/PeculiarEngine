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
	class SightMoveEvent;
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

signals:
//	void syncObject(void);
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
	Array<Object*> object;
	Array<Force*> force;
	Array<Event*> event;


	short deadObjectIndex[100];
	short deadObjectNum;
	short autoGenerationIndex;
};

#endif
