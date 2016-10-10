#ifndef FIELD_H
#define FIELD_H

#include <QObject>

class Sight;
class Object;
class QTimer;

class Field : public QObject//time loss?
{
	Q_OBJECT
public:
	Field(void);
	~Field(void);
	void open(void);
signals:
	void reportScore(short);
public slots:
	void timeControl(void);
	void update(void);
	void autoGenerate(void);
private:
	void objectGenerate(Object*);
	bool interference(short , short);
	char judgeCrash(short , short);
	void reflect(short , short);
	bool cubicEquation(float* , float* , float* , float* , float*);
	bool calculate1(float* , float* , float*);

	Sight* sight;
	QTimer* time;
	QTimer* dominatorTime;
	QTimer* autoGeneration;
	Object** object;

	short objectNum;
	char** stateCrash;
	short** indexCrash;

	short deadObjectIndex[100];
	short deadObjectNum;
	short autoGenerationIndex;
};

#endif
