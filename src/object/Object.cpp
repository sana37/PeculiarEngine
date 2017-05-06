#include "Object.h"
#include "Calculater.h"
#include <stdio.h>
#include <math.h>

Object::Object(const char* fileName) :
	velocity(Vector()),
	omegaVector(Vector(0, 1, 0)),
	omega(0),
	gravityCenter(Vector())
{
	radius = 0;
	isDominated = false;

	FILE* fp = fopen(fileName, "r");

	if (fp != NULL) {
		while (fgetc(fp) != ':') ;
		if (fscanf(fp, "%hd", &vertexNum) == EOF)
			printf("ERROR\n");

		while (fgetc(fp) != '+') ;
		vertex = new Vector[vertexNum];
		vertexEmbodyFlag = new bool[vertexNum];
		short realVertexNum = 0;
		for (short i = 0  ;  i < vertexNum  ;  i++) {
			float temp[3];

			if (fscanf(fp, "%f%f%f", &temp[0], &temp[1], &temp[2]) == EOF) {
				printf("ERROR\n");
				break;
			}

			vertex[i].setVector(temp);

			vertexEmbodyFlag[i] = true;

			while (fgetc(fp) != '/') ;
			while (1) {
				char ch = fgetc(fp);
				if (ch == '/')
					break;
				if (ch == '|')
					vertexEmbodyFlag[i] = false;
			}

			if (vertexEmbodyFlag[i] == true) {
				gravityCenter += Vector(temp);
				realVertexNum++;
			}
		}
		gravityCenter /= realVertexNum;



		while (fgetc(fp) != '*') ;


		while (fgetc(fp) != ':') ;
		if (fscanf(fp, "%hd", &lineNum) == EOF)
			printf("ERROR\n");

		lineLVertexIndex = new short[lineNum];
		lineRVertexIndex = new short[lineNum];

		while (fgetc(fp) != '+') ;
		for (short i = 0  ;  i < lineNum  ;  i++) {
			if (fscanf(fp, "%hd%hd", &lineLVertexIndex[i], &lineRVertexIndex[i]) == EOF) break;
			lineLVertexIndex[i]--;
			lineRVertexIndex[i]--;

			while (fgetc(fp) != '/') ;
			while (fgetc(fp) != '/') ;
		}


		while (fgetc(fp) != '*') ;


		while (fgetc(fp) != ':') ;
		if (fscanf(fp, "%hd", &polygonNum) == EOF)
			printf("ERROR\n");

		polygon1VertexIndex = new short[polygonNum];
		polygon2VertexIndex = new short[polygonNum];
		polygon3VertexIndex = new short[polygonNum];
		polygonR = new short[polygonNum];
		polygonG = new short[polygonNum];
		polygonB = new short[polygonNum];
		polygonEmbodyFlag = new bool[polygonNum];

		while (fgetc(fp) != '+') ;
		for (short i = 0  ;  i < polygonNum  ;  i++) {
			float colorR, colorG, colorB;

			if (fscanf(fp, "%hd%hd%hd", &polygon1VertexIndex[i], &polygon2VertexIndex[i], &polygon3VertexIndex[i]) == EOF) break;
			polygon1VertexIndex[i]--;
			polygon2VertexIndex[i]--;
			polygon3VertexIndex[i]--;

			while (fgetc(fp) != '|') ;

			if (fscanf(fp, "%f%f%f", &colorR, &colorG, &colorB) == EOF) break;
			polygonR[i] = 32767 * colorR;
			polygonG[i] = 32767 * colorG;
			polygonB[i] = 32767 * colorB;

			polygonEmbodyFlag[i] = true;
			while (fgetc(fp) != '/') ;
			while (1) {
				char ch = fgetc(fp);
				if (ch == '/')
					break;
				if (ch == '|')
					polygonEmbodyFlag[i] = false;
			}
		}


		while (fgetc(fp) != '*') ;

		char next = fgetc(fp);

		if (next == 'm') {
			while (fgetc(fp) != ':') ;
			if (fscanf(fp, "%f", &mass) == EOF)
				printf("ERROR\n");

			while (fgetc(fp) != '*') ;
			next = fgetc(fp);
		} else {
			mass = 1;
		}


		if (next == 'v') {
			float temp[3];
			if (fscanf(fp, "%f%f%f", &temp[0], &temp[1], &temp[2]) == EOF) {
				velocity.setVector(0, 0, 0);
			} else {
				velocity.setVector(temp);
			}
		}

		fclose(fp);

		for (short i = 0  ;  i < vertexNum  ;  i++) {
			if (vertexEmbodyFlag[i] == false)
				continue;
			if (radius < (vertex[i] - gravityCenter).getMagnitude())
				radius = (vertex[i] - gravityCenter).getMagnitude();
		}
	} else {
		printf("I cannot open such a file\n");
		vertexNum = -1;
		lineNum = -1;
		polygonNum = -1;
		//radius = 10000;
		mass = 1;
	}
}

Object::Object(const Object& _object)
{
	vertexNum = _object.vertexNum;
	lineNum = _object.lineNum;
	polygonNum = _object.polygonNum;

	vertex = new Vector[vertexNum];
	gravityCenter = _object.gravityCenter;
	vertexEmbodyFlag = new bool[vertexNum];

	for (short i = 0  ;  i < vertexNum  ;  i++) {
		vertex[i] = _object.vertex[i];
		vertexEmbodyFlag[i] = _object.vertexEmbodyFlag[i];
	}


	lineLVertexIndex = new short[lineNum];
	lineRVertexIndex = new short[lineNum];
	for (short i = 0  ;  i < lineNum  ;  i++) {
		lineLVertexIndex[i] = _object.lineLVertexIndex[i];
		lineRVertexIndex[i] = _object.lineRVertexIndex[i];
	}


	polygon1VertexIndex = new short[polygonNum];
	polygon2VertexIndex = new short[polygonNum];
	polygon3VertexIndex = new short[polygonNum];
	polygonR = new short[polygonNum];
	polygonG = new short[polygonNum];
	polygonB = new short[polygonNum];
	polygonEmbodyFlag = new bool[polygonNum];

	for (short i = 0  ;  i < polygonNum  ;  i++) {
		polygon1VertexIndex[i] = _object.polygon1VertexIndex[i];
		polygon2VertexIndex[i] = _object.polygon2VertexIndex[i];
		polygon3VertexIndex[i] = _object.polygon3VertexIndex[i];
		polygonR[i] = _object.polygonR[i];
		polygonG[i] = _object.polygonG[i];
		polygonB[i] = _object.polygonB[i];
		polygonEmbodyFlag[i] = _object.polygonEmbodyFlag[i];
	}


	velocity = _object.velocity;
	omegaVector = _object.omegaVector;
	omega = _object.omega;
	radius = _object.radius;
	mass = _object.mass;

	isDominated = _object.isDominated;
}

Object::~Object(void)
{
	delete[] vertex;
	delete[] lineLVertexIndex;
	delete[] lineRVertexIndex;
	delete[] polygon1VertexIndex;
	delete[] polygon2VertexIndex;
	delete[] polygon3VertexIndex;
	delete[] polygonR;
	delete[] polygonG;
	delete[] polygonB;
	delete[] vertexEmbodyFlag;
	delete[] polygonEmbodyFlag;
}

void Object::composeObject(Object* material)	//atode nakusu.  vertex nadoga dokuritusitamama unndouno eikyouwo tomoni ukeru sikumini sitai
{
	Vector* tempVertex = new Vector[vertexNum + material->getVertexNum()];
	short* tempPolygon1VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygon2VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygon3VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonR = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonG = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonB = new short[polygonNum + material->getPolygonNum()];
	bool* tempVertexEmbodyFlag = new bool[vertexNum + material->getVertexNum()];
	bool* tempPolygonEmbodyFlag = new bool[polygonNum + material->getPolygonNum()];

	for (short i = 0  ;  i < vertexNum + material->getVertexNum()  ;  i++) {
		if (i < vertexNum) {
			tempVertex[i] = vertex[i];
			tempVertexEmbodyFlag[i] = vertexEmbodyFlag[i];
		} else {
			tempVertex[i] = material->getVertex(i - vertexNum);
			tempVertexEmbodyFlag[i] = false;//////////////
		}
	}
	delete[] vertex;
	delete[] vertexEmbodyFlag;
	vertex = tempVertex;
	vertexEmbodyFlag = tempVertexEmbodyFlag;

	for (short i = 0  ;  i < polygonNum + material->getPolygonNum()  ;  i++) {
		if (i < polygonNum) {
			tempPolygon1VertexIndex[i] = polygon1VertexIndex[i];
			tempPolygon2VertexIndex[i] = polygon2VertexIndex[i];
			tempPolygon3VertexIndex[i] = polygon3VertexIndex[i];
			tempPolygonR[i] = polygonR[i];
			tempPolygonG[i] = polygonG[i];
			tempPolygonB[i] = polygonB[i];
			tempPolygonEmbodyFlag[i] = polygonEmbodyFlag[i];
		} else {
			tempPolygon1VertexIndex[i] = vertexNum + material->polygon1VertexIndex[i - polygonNum];
			tempPolygon2VertexIndex[i] = vertexNum + material->polygon2VertexIndex[i - polygonNum];
			tempPolygon3VertexIndex[i] = vertexNum + material->polygon3VertexIndex[i - polygonNum];
			tempPolygonR[i] = 0;
			tempPolygonG[i] = 0;
			tempPolygonB[i] = 0;
			tempPolygonEmbodyFlag[i] = false;/////////////
		}
	}
	vertexNum += material->getVertexNum();
	polygonNum += material->getPolygonNum();
	delete[] polygon1VertexIndex;
	delete[] polygon2VertexIndex;
	delete[] polygon3VertexIndex;
	delete[] polygonR;
	delete[] polygonG;
	delete[] polygonB;
	delete[] polygonEmbodyFlag;
	polygon1VertexIndex = tempPolygon1VertexIndex;
	polygon2VertexIndex = tempPolygon2VertexIndex;
	polygon3VertexIndex = tempPolygon3VertexIndex;
	polygonR = tempPolygonR;
	polygonG = tempPolygonG;
	polygonB = tempPolygonB;
	polygonEmbodyFlag = tempPolygonEmbodyFlag;
}



short Object::getVertexNum(void) const
{
	return vertexNum;
}

short Object::getPolygonNum(void) const
{
	return polygonNum;
}

short Object::getLineNum(void) const
{
	return lineNum;
}


const Vector& Object::getVertex(short vertexIndex) const
{
	return vertex[vertexIndex];
}

const Vector& Object::getGravityCenter(void) const
{
	return gravityCenter;
}

const Vector& Object::getPolygon1Vertex(short polygonIndex) const
{
	return this->getVertex(polygon1VertexIndex[polygonIndex]);
}

const Vector& Object::getPolygon2Vertex(short polygonIndex) const
{
	return this->getVertex(polygon2VertexIndex[polygonIndex]);
}

const Vector& Object::getPolygon3Vertex(short polygonIndex) const
{
	return this->getVertex(polygon3VertexIndex[polygonIndex]);
}

const Vector& Object::getLineLVertex(short lineIndex) const
{
	return this->getVertex(lineLVertexIndex[lineIndex]);
}

const Vector& Object::getLineRVertex(short lineIndex) const
{
	return this->getVertex(lineRVertexIndex[lineIndex]);
}


float Object::getRadius(void) const
{
	return radius;
}

float Object::getMass(void)
{
	return mass;
}

const Vector& Object::getVelocity(void) const
{
	return velocity;
}

Vector Object::getOmega(void) const
{
	return omegaVector * omega;
}


short Object::getPolygonR(short num) const
{
	return polygonR[num];
}

short Object::getPolygonG(short num) const
{
	return polygonG[num];
}

short Object::getPolygonB(short num) const
{
	return polygonB[num];
}

char Object::whichClass(void)
{
	return 'O';
}

bool Object::isActive(void)
{
	return (velocity.getMagnitude() != 0);
}

bool Object::isVertexEmbody(short vertexIndex) const
{
	return vertexEmbodyFlag[vertexIndex];
}

bool Object::isPolygonEmbody(short polygonIndex) const
{
	return polygonEmbodyFlag[polygonIndex];
}




void Object::setVertex(short num, const Vector& vector)
{
	vertex[num] = vector;
}

void Object::setVertex(short num, float x, float y, float z)
{
	vertex[num].setVector(x, y, z);
}

void Object::setGravityCenter(const Vector& vector)
{
	gravityCenter = vector;
}

void Object::setGravityCenter(float x, float y, float z)
{
	gravityCenter.setVector(x, y, z);
}

void Object::setVelocity(const Vector& vector)
{
	velocity = vector;
}

void Object::setVelocity(float x, float y, float z)
{
	velocity.setVector(x, y, z);
}

void Object::setOmega(const Vector& vector)
{
	omega = vector.getMagnitude();
	if (omega != 0)
		omegaVector = vector / omega;
}

void Object::setOmega(float x, float y, float z)
{
	Vector vector(x, y, z);
	setOmega(vector);
}

void Object::setDomination(bool originalIsDominated)
{
	isDominated = originalIsDominated;
}

bool Object::update(void)
{
	this->run();
	this->rotate();
	return false;//whether velocity changes or not
}

void Object::run(void)
{
	for (short i = 0  ;  i < vertexNum  ;  i++) {
		vertex[i] += velocity;
	}
	gravityCenter += velocity;
}

void Object::back(void)
{
	for (short i = 0  ;  i < vertexNum  ;  i++) {
		vertex[i] -= velocity;
	}
	gravityCenter -= velocity;
}

void Object::stop(void)
{
	velocity.setVector(0, 0, 0);
	omega = 0;
}

void Object::moveRelative(const Vector& vector)
{
	for (short i = 0  ;  i < vertexNum  ;  i++) {
		vertex[i] += vector;
	}
	gravityCenter += vector;
}

void Object::moveRelative(float x, float y, float z)
{
	Vector vector(x, y, z);
	this->moveRelative(vector);
}

void Object::moveAbsolute(const Vector& vector)
{
	Vector temp = vector - gravityCenter;
	this->moveRelative(temp);
}

void Object::moveAbsolute(float x, float y, float z)
{
	Vector vector(x, y, z);
	this->moveAbsolute(vector);
}

void Object::rotate(void)
{
	for (short i = 0  ;  i < vertexNum  ;  i++) {
		Calculater::rotate(&vertex[i], gravityCenter, omegaVector, omega);
	}
}

void Object::push(Vector vector)
{
	velocity += (vector / mass);
}

void Object::accelerate(Vector vector)
{
	velocity += vector;
}

void Object::applyTorque(Vector torque)
{
	omegaVector *= omega;
	omegaVector += (torque / (mass / 1.0));
	omega = omegaVector.getMagnitude();
	if (omega != 0)
		omegaVector /= omega;
}

void Object::enblack(short num)
{
	polygonR[num] = 0;
	polygonG[num] = 0;
	polygonB[num] = 0;
}
/*
void Object::debug(void)
{
	printf("%f : %f  %f  %f;\n", omega, omegaVector.getX(), omegaVector.getY(), omegaVector.getZ());
}
*/