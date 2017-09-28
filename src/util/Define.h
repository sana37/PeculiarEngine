#ifndef DEFINE_H
#define DEFINE_H

/*Field*/
//#define OBJECT_NUM	7
#define TIME_UNIT	10	//depended on by GRAVITY

/*Sight*/
#define SPEED		0.15
#define DOMAIN_MAX	45//29.85
#define CEILING		60
#define FLOOR		-5//0.15
#define BAR_POS		29.85
#define OMEGA		0.02

/*CrashKeeper*/
#define ATTACH_PAIR_MAX		100


/*Object*/
#define MOMENT_PER_MASS		1	//2

/*ObjectStatus*/
#define FIRST_ATTACH_MAX	10


/*Gravity*/
#define GRAVITY		0.004
//#define GRAVITY		0.098//9.8 * TIMEUNIT * 1000


/*CrashEvent*/
#define ZERO_VELOCITY		0//0.05//too big
#define ZERO_ACCELERATION	0.0001

/*MoveEvent*/
#define WORLD		80


/*Array*/
#define INITIAL_MAX	50

/*Calculater*/
#define PI 3.141592


#endif
