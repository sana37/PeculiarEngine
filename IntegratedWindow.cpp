#include "IntegratedWindow.h"
#include "Field.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QString>
#include <stdio.h>

IntegratedWindow::IntegratedWindow(void)
{
	title = new QLabel;
	label = new QLabel;
	button = new QPushButton;
	layoutH = new QHBoxLayout;
	layout = new QVBoxLayout;
	clock = new QTimer;

	timeCount = 60;
	isPlay = 0;
	score = 0;

	layoutH->addWidget(button);
	layout->addWidget(title);
	layout->addWidget(label);
	layout->addLayout(layoutH);
	this->setLayout(layout);

	connect(button , SIGNAL(clicked()) , this , SLOT(changePlay()));

	this->setup();
	this->show();
}

void IntegratedWindow::changePlay(void)
{
	switch(isPlay){
		case 0 : {
			this->start();
			break;
		}
		case 1 : {
			this->endGame();
			break;
		}
		case 2 : {
			this->setup();
			disconnect(clock , SIGNAL(timeout()) , this , SLOT(changePlay()));
			clock->stop();
			break;
		}
	}
}

void IntegratedWindow::setup(void)
{
	field = new Field;
	field->open();
	title->setText("3D素数シューティングゲーム");
	QFont font = title->font();
	font.setPointSize(40);
	title->setFont(font);
	label->setText("ご自由にプレイしてください。エンターキーを押すとスタートします。\
\n[ゲーム概要]\n１を除く非素数が書かれた立方体を撃つと、得点になります。\
\n１または素数を撃つと減点になります。\nまた、撃たれた数字は２つの数字へと分裂し、それらはもとの数の約数です。\
\n同じものを打ち続けていると１や素数になり、減点になってしまうので気をつけてください。\n[操作説明]\
\n↑ ↓ → ← キーを押すと、それぞれの向きへと方向転換します。スペースキーを押すと弾を発射します。\
\nＷキーを押すと、向いている方向へと進みます。Ｓキーを押すと、反対向きに退きます。");
	button->setText("始める");
	this->move(10 , 200);
	this->resize(600 , 300);
	this->hide();
	this->show();
	isPlay = 0;
	score = 0;
}

void IntegratedWindow::start(void)
{
	title->setText("");
	label->setText("残り時間:60");
	button->setText("やめる");
//	this->close();
	this->move(150 , 300);
	this->resize(200 , 150);
//	this->show();
	timeCount = 60;
	isPlay = 1;
	delete field;
	field = new Field;
	clock->start(1000);
	connect(clock , SIGNAL(timeout()) , this , SLOT(tick()));
	connect(field , SIGNAL(reportScore(short)) , this , SLOT(scoring(short)));
	field->open();
}

void IntegratedWindow::endGame(void)
{
//	FILE* fp = fopen("score" , "r");
	QString str1;
	QString str2;

	str1 = "終了です。\nスコア:";
	str2.setNum(score);

	str1 += str2;

	clock->stop();
	disconnect(clock , SIGNAL(timeout()) , this , SLOT(tick()));
	disconnect(field , SIGNAL(reportScore(short)) , this , SLOT(scoring(short)));
	clock->start(10000);
	connect(clock , SIGNAL(timeout()) , this , SLOT(changePlay()));
	delete field;
	label->setText(str1);
	button->setText("OK");
	isPlay = 2;
/*
	if(fp != NULL){
		short num;
		while(fgetc(fp) != '@');
		if(fscanf(fp , "%hd" , &num) == EOF)
			printf("ERROR\n");
		short yourScoreIndex = num;
		int scores[num];

		for(short i = 0  ;  i < num  ;  i++){
			if(fscanf(fp , "%d" , &scores[i]) == EOF)
				printf("ERROR\n");
		}
		fclose(fp);

		fp = fopen("score" , "w");
		fputc('@' , fp);
		fprintf(fp , "%d\n" , num + 1);
		for(short i = 0  ;  i < num + 1  ;  i++){
			if(score > scores[i]  ||  i == num){
				fprintf(fp , " %d" , 123);
				yourScoreIndex = i;
				score = -1;
				i++;
			}
			fprintf(fp , " %d " , scores[i]);
		}
		fclose(fp);

//		printf("あなたはスコアは%d人中%d番目です。\n" , num + 1 , yourScoreIndex + 1);
	}else{
		printf("can't open score file\n");
	}
*/
	this->move(600 , 300);
	this->show();
}

void IntegratedWindow::tick(void)
{
	QString str1;
	QString str2;

	str1 = "残り時間:";
	timeCount--;
	str2.setNum(timeCount);
	str1 += str2;

	str2 = "\nスコア:";
	str1 += str2;

	str2.setNum(score);
	str1 += str2;

	label->setText(str1);
	this->move(150 , 300);
	this->resize(200 , 150);

	if(timeCount <= 0){
		endGame();
	}
}

void IntegratedWindow::scoring(short result)
{
	QString str1;
	QString str2;

	if(result == 1)
		score += 100;
	else if(result == 0)
		score -= 50;

	str1 = "残り時間:";
	str2.setNum(timeCount);
	str1 += str2;

	str2 = "\nスコア:";
	str1 += str2;

	str2.setNum(score);
	str1 += str2;

	label->setText(str1);
	this->move(150 , 300);
	this->resize(200 , 150);
}
