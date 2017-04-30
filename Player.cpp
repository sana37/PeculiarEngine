#include "Player.h"
#include "Vector.h"
#include <stdio.h>

Player::Player(const char* fileName) : Object::Object(fileName)
{
//	classCode = 'P';
	FILE* fp = fopen(fileName, "r");

	if (fp != NULL) {
		char ch;
		while (1) {
			ch = fgetc(fp);
			if (ch == '@'  ||  ch == EOF)
				break;
		}

		if (ch != EOF) {
			while (fgetc(fp) != ':') ;
			if (fscanf(fp, "%hd", &routeNum) == EOF)
				printf("ERROR\n");

			while (fgetc(fp) != '+') ;
			route = new Vector[routeNum];
			for (short i = 0  ;  i < routeNum  ;  i++) {
				float temp[3];

				if (fscanf(fp, "%f%f%f", &temp[0], &temp[1], &temp[2]) == EOF) {
					printf("ERROR\n");
					break;
				}

				route[i].setVector(temp);
			}
			routeIndex = 0;
		} else {
			routeNum = 0;
			routeIndex = 0;
			printf("route does not exist.\n");
		}

		fclose(fp);

	}
}

Player::Player(const Player& player) : Object::Object(player)
{
//	classCode = 'P';
	routeNum = player.routeNum;
	routeIndex = player.routeIndex;
	route = new Vector[routeNum];
	for (short i = 0  ;  i < routeNum  ;  i++)
		route[i] = player.route[i];
}

bool Player::updatePlayer(void)
{
	this->decelerate();
	if (isDominated == false)
		this->autoMove();
	this->run();

	return true;
}

void Player::decelerate(void)
{
	velocity *= 0.98;
	if (velocity.getMagnitude() < 0.001)
		this->stop();
}

void Player::autoMove(void)
{
	Vector temp;

	temp = route[routeIndex] - this->getGravityCenter();
	if (temp.getMagnitude() < 0.5) {
		temp.setVector(0, 0, 0);
		routeIndex++;
		if (routeIndex >= routeNum)
			routeIndex = 0;
	} else {
		temp *= (0.05 / temp.getMagnitude());
	}
	velocity += temp;
}

char Player::whichClass(void)
{
	return 'P';
}
