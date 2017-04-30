#ifndef INTEGRATEDWINDOW_H
#define INTEGRATEDWINDOW_H

#include <QDialog>

class Field;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;

class IntegratedWindow : public QDialog
{
	Q_OBJECT

public:
	IntegratedWindow(void);

public slots:
	void changePlay(void);
	void tick(void);
	void scoring(short);

private:
	void start(void);
	void endGame(void);
	void setup(void);


	Field* field;
	QLabel* title;
	QLabel* label;
	QPushButton* button;
	QHBoxLayout* layoutH;
	QVBoxLayout* layout;
	QTimer* clock;

	short timeCount;
	short isPlay;
	int score;
};

#endif
