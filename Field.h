#ifndef FIELD_H
#define FIELD_H

#include <QObject>

#define OBJECT_NUM	7
#define EVENT_NUM	3
#define TIME_UNIT	10
#define WORLD		80

class Sight;
class Object;
class QTimer;

class Field : public QObject
{
	Q_OBJECT
private:
//singleton object field
	static Field* field;

	Field(void);
	Field(const Field&);
	~Field(void);
public:
	static Field* getInstance(void);
	static void deleteInstance(void);

public:
	void open(void);
signals:
	void reportScore(short);
public slots:
	void execTimeEvent(void);
	void timeControl(void);
	void autoGenerate(void);
private:
	void objectGenerate(Object*);

	class Event;
	class MoveEvent;
	class SightMoveEvent;
	class CrashEvent;

	Sight* sight;
	QTimer* time;
	QTimer* autoGeneration;
	Object** object;
	Event** event;

	short objectNum;
	char** stateCrash;
	short** indexCrash;
	short eventNum;

	short deadObjectIndex[100];
	short deadObjectNum;
	short autoGenerationIndex;
};

#endif
