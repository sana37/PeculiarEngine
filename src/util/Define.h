#ifndef DEFINE_H
#define DEFINE_H

/*Field*/
//#define OBJECT_NUM	7
#define EVENT_NUM	4
#define TIME_UNIT	10	//depended on by GRAVITY

/*Sight*/
#define SPEED		0.15
#define DOMAIN_MAX	29.85
#define OMEGA		0.01

/*CrashKeeper*/
#define ATTACH_PAIR_MAX		100


/*ObjectStatus*/
#define FIRST_ATTACH_MAX	10


/*Impulse*/
#define LEAST_DIST	0.05
#define K1			0.000001

/*Gravity*/
#define GRAVITY		0.004
//#define GRAVITY		0.098//9.8 * TIMEUNIT * 1000


/*CrashEvent*/
#define NEAR_ZERO	0.03

/*MoveEvent*/
#define WORLD		80


/*Array*/
#define INITIAL_MAX	50

/*Calculater*/
#define PI 3.141592


#endif
