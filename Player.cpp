#include "Player.h"
#include "Vector.h"
#include <stdio.h>

Player::Player(const char* fileName) : Object::Object(fileName)
{
	classCode = 'P';
	FILE* fp = fopen(fileName , "r");

	if(fp != NULL){
		char ch;
		while(1){
			ch = fgetc(fp);
			if(ch == '@'  ||  ch == EOF)
				break;
		}

		if(ch != EOF){
			while(fgetc(fp) != ':');
			if(fscanf(fp , "%hd" , &routeNum) == EOF)
				printf("ERROR\n");

			while(fgetc(fp) != '+');
			route = new Vector*[routeNum];
			for(short i = 0  ;  i < routeNum  ;  i++){
				float temp[3];

				if(fscanf(fp , "%f%f%f" , &temp[0] , &temp[1] , &temp[2]) == EOF){
					printf("ERROR\n");
					break;
				}

				route[i] = new Vector(temp);
			}
			routeIndex = 0;
		}else{
			routeNum = 0;
			routeIndex = 0;
			printf("route does not exist.\n");
		}

		fclose(fp);

	}
}

Player::Player(const Player& originalPlayer) : Object::Object(originalPlayer)
{
//	classCode = 'P';
	if(originalPlayer.whichClass() != 'O'){
		Vector** originalRoute;

		originalPlayer.inheritPlayer(&routeNum , &routeIndex , &originalRoute);
		route = new Vector*[routeNum];
		for(short i = 0  ;  i < routeNum  ;  i++)
			route[i] = new Vector(*originalRoute[i]);
	}else{
		printf("copy Object -> Player or more\n");
		routeNum = 0;
		routeIndex = 0;
	}
}

void Player::inheritPlayer(short* replicaRouteNum , short* replicaRouteIndex , Vector*** replicaRoute) const//!!
{
	*replicaRouteNum = routeNum;
	*replicaRouteIndex = routeIndex;
	*replicaRoute = route;//!!
}

bool Player::updatePlayer(void)
{
	this->decelerate();
	if(isDominated == false)
		this->autoMove();
	this->move();
	return(true);
}

void Player::decelerate(void)
{
	velocity->multiply(0.98);
	if(velocity->getMagnitude() < 0.001){
		this->stop();
	}
}

void Player::autoMove(void)
{
	Vector temp;

	temp = (*route[routeIndex] - this->getGravityCenter());
	if(temp.getMagnitude() < 0.5){
		temp.setVector(0 , 0 , 0);
		routeIndex++;
		if(routeIndex >= routeNum)
			routeIndex = 0;
	}else{
		temp = temp * (0.05 / temp.getMagnitude());
	}
	*velocity = *velocity + temp;
}
