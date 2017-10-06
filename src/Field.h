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

signals:
//	void syncObject(void);
	void reportScore(short);

public slots:
	void execTimeEvent(void);
	void timeControl(void);
	void autoGenerate(void);

	void addObject(Object*);
	void deleteObject(Object*);
	void addForce(Force*);
	void deleteForce(Object*);

private:
	Sight* sight;
	QTimer* time;
	QTimer* autoGeneration;
	Array<Object*> object;
	Array<Force*> force;
	Event* forceEvent;
	Event* crashEvent;
	Event* moveEvent;


	short deadObjectIndex[100];
	short deadObjectNum;
	short autoGenerationIndex;
};

#endif
