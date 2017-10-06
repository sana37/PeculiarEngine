#include "Object.h"
#include "ObjectStatus.h"
#include "Calculater.h"
#include "Define.h"
#include <stdio.h>
#include <math.h>

Object::Object(const char* fileName) :
	velocity(Vector()),
	omegaVector(Vector(0, 1, 0)),
	omega(0),
	gravityCenter(Vector()),
	fixed(false),
	name(fileName)
{
	radius = 0;
	isDominated = false;
	status = new ObjectStatus();

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
		polygonInsideFlag = new bool[polygonNum];

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

		for (short i = 0; i < polygonNum; i++) {
			short penetratedPlgnNum = 0;
			const Vector a1 = getPolygon1Vertex(i);
			const Vector a2 = getPolygon2Vertex(i);
			const Vector a3 = getPolygon3Vertex(i);
			Vector n = (a2 - a1) % (a3 - a1);

			for (short j = 0; j < polygonNum; j++) {
				if (i == j  ||  polygonEmbodyFlag[j] == false)
					continue;

				const Vector b1 = getPolygon1Vertex(j);
				const Vector b2 = getPolygon2Vertex(j);
				const Vector b3 = getPolygon3Vertex(j);
				Vector solution;

				if (Calculater::solveCubicEquation(
					b2 - b1,
					b3 - b1,
					n * -1.0,
					(a1 + (a2 * 2) + (a3 * 3)) / 6.0 - b1,
					&solution
				)) {
					if (0 <= solution.getX()  &&
						0 <= solution.getY()  &&
						solution.getX() + solution.getY() <= 1  &&
						0 < solution.getZ()
					) {
						penetratedPlgnNum++;
					}
				}
			}

			polygonInsideFlag[i] = penetratedPlgnNum % 2;
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

		inertiaMoment = mass * MOMENT_PER_MASS;

		if (next == 'v') {
			float temp[3];
			if (fscanf(fp, "%f%f%f", &temp[0], &temp[1], &temp[2]) == EOF) {
				velocity.setVector(0, 0, 0);
			} else {
				velocity.setVector(temp);
			}
		}

		fclose(fp);

		reloadRadius();
	} else {
		printf("I cannot open such a file\n");
		vertexNum = -1;
		lineNum = -1;
		polygonNum = -1;
		//radius = 10000;
		mass = 1;
		inertiaMoment = mass * MOMENT_PER_MASS;
	}
}

Object::Object(const Object& object) : name(object.name)
{
	vertexNum = object.vertexNum;
	lineNum = object.lineNum;
	polygonNum = object.polygonNum;

	vertex = new Vector[vertexNum];
	gravityCenter = object.gravityCenter;
	vertexEmbodyFlag = new bool[vertexNum];

	for (short i = 0  ;  i < vertexNum  ;  i++) {
		vertex[i] = object.vertex[i];
		vertexEmbodyFlag[i] = object.vertexEmbodyFlag[i];
	}


	lineLVertexIndex = new short[lineNum];
	lineRVertexIndex = new short[lineNum];
	for (short i = 0  ;  i < lineNum  ;  i++) {
		lineLVertexIndex[i] = object.lineLVertexIndex[i];
		lineRVertexIndex[i] = object.lineRVertexIndex[i];
	}


	polygon1VertexIndex = new short[polygonNum];
	polygon2VertexIndex = new short[polygonNum];
	polygon3VertexIndex = new short[polygonNum];
	polygonR = new short[polygonNum];
	polygonG = new short[polygonNum];
	polygonB = new short[polygonNum];
	polygonEmbodyFlag = new bool[polygonNum];
	polygonInsideFlag = new bool[polygonNum];

	for (short i = 0  ;  i < polygonNum  ;  i++) {
		polygon1VertexIndex[i] = object.polygon1VertexIndex[i];
		polygon2VertexIndex[i] = object.polygon2VertexIndex[i];
		polygon3VertexIndex[i] = object.polygon3VertexIndex[i];
		polygonR[i] = object.polygonR[i];
		polygonG[i] = object.polygonG[i];
		polygonB[i] = object.polygonB[i];
		polygonEmbodyFlag[i] = object.polygonEmbodyFlag[i];
		polygonInsideFlag[i] = object.polygonInsideFlag[i];
	}


	velocity = object.velocity;
	omegaVector = object.omegaVector;
	omega = object.omega;
	radius = object.radius;
	mass = object.mass;
	inertiaMoment = object.inertiaMoment;

	fixed = object.fixed;
	isDominated = object.isDominated;
	status = new ObjectStatus(*(object.status));
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
	delete[] polygonInsideFlag;
	delete status;
}

void Object::composeObject(Object* material)	//atode nakusu.  vertex nadoga dokuritusitamama unndouno eikyouwo tomoni ukeru sikumini sitai
{
	Vector* tempVertex = new Vector[vertexNum + material->getVertexNum()];
	short* tempLineLVertexIndex = new short[lineNum + material->getLineNum()];
	short* tempLineRVertexIndex = new short[lineNum + material->getLineNum()];
	short* tempPolygon1VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygon2VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygon3VertexIndex = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonR = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonG = new short[polygonNum + material->getPolygonNum()];
	short* tempPolygonB = new short[polygonNum + material->getPolygonNum()];
	bool* tempVertexEmbodyFlag = new bool[vertexNum + material->getVertexNum()];
	bool* tempPolygonEmbodyFlag = new bool[polygonNum + material->getPolygonNum()];
	bool* tempPolygonInsideFlag = new bool[polygonNum + material->getPolygonNum()];

	for (short i = 0; i < vertexNum + material->getVertexNum(); i++) {
		if (i < vertexNum) {
			tempVertex[i] = vertex[i];
			tempVertexEmbodyFlag[i] = vertexEmbodyFlag[i];
		} else {
			tempVertex[i] = material->getVertex(i - vertexNum);
			tempVertexEmbodyFlag[i] = material->isVertexEmbody(i - vertexNum);
		}
	}
	delete[] vertex;
	delete[] vertexEmbodyFlag;
	vertex = tempVertex;
	vertexEmbodyFlag = tempVertexEmbodyFlag;

	for (short i = 0; i < lineNum + material->getLineNum(); i++) {
		if (i < lineNum) {
			tempLineLVertexIndex[i] = lineLVertexIndex[i];
			tempLineRVertexIndex[i] = lineRVertexIndex[i];
		} else {
			tempLineLVertexIndex[i] = vertexNum + material->lineLVertexIndex[i - lineNum];
			tempLineRVertexIndex[i] = vertexNum + material->lineRVertexIndex[i - lineNum];
		}
	}
	delete[] lineLVertexIndex;
	delete[] lineRVertexIndex;
	lineLVertexIndex = tempLineLVertexIndex;
	lineRVertexIndex = tempLineRVertexIndex;

	for (short i = 0; i < polygonNum + material->getPolygonNum(); i++) {
		if (i < polygonNum) {
			tempPolygon1VertexIndex[i] = polygon1VertexIndex[i];
			tempPolygon2VertexIndex[i] = polygon2VertexIndex[i];
			tempPolygon3VertexIndex[i] = polygon3VertexIndex[i];
			tempPolygonR[i] = polygonR[i];
			tempPolygonG[i] = polygonG[i];
			tempPolygonB[i] = polygonB[i];
			tempPolygonEmbodyFlag[i] = polygonEmbodyFlag[i];
			tempPolygonInsideFlag[i] = polygonInsideFlag[i];
		} else {
			tempPolygon1VertexIndex[i] = vertexNum + material->polygon1VertexIndex[i - polygonNum];
			tempPolygon2VertexIndex[i] = vertexNum + material->polygon2VertexIndex[i - polygonNum];
			tempPolygon3VertexIndex[i] = vertexNum + material->polygon3VertexIndex[i - polygonNum];
			tempPolygonR[i] = material->polygonR[i - polygonNum];
			tempPolygonG[i] = material->polygonG[i - polygonNum];
			tempPolygonB[i] = material->polygonB[i - polygonNum];
			tempPolygonEmbodyFlag[i] = material->isPolygonEmbody(i - polygonNum);
			tempPolygonInsideFlag[i] = material->isPolygonInside(i - polygonNum);
		}
	}
	vertexNum += material->getVertexNum();
	lineNum += material->getLineNum();
	polygonNum += material->getPolygonNum();
	delete[] polygon1VertexIndex;
	delete[] polygon2VertexIndex;
	delete[] polygon3VertexIndex;
	delete[] polygonR;
	delete[] polygonG;
	delete[] polygonB;
	delete[] polygonEmbodyFlag;
	delete[] polygonInsideFlag;
	polygon1VertexIndex = tempPolygon1VertexIndex;
	polygon2VertexIndex = tempPolygon2VertexIndex;
	polygon3VertexIndex = tempPolygon3VertexIndex;
	polygonR = tempPolygonR;
	polygonG = tempPolygonG;
	polygonB = tempPolygonB;
	polygonEmbodyFlag = tempPolygonEmbodyFlag;
	polygonInsideFlag = tempPolygonInsideFlag;
//	gravity center is not change??
//	radius change??
}

Object* Object::decomposeObject(int vertexId, int polygonId, int lineId, const char* name)
{
	Object* obj = new Object(name);
	obj->velocity.setVector(0, 0, 0);

	Vector* tempVertex = new Vector[vertexId];
	short* tempLineLVertexIndex = new short[lineId];
	short* tempLineRVertexIndex = new short[lineId];
	short* tempPolygon1VertexIndex = new short[polygonId];
	short* tempPolygon2VertexIndex = new short[polygonId];
	short* tempPolygon3VertexIndex = new short[polygonId];
	short* tempPolygonR = new short[polygonId];
	short* tempPolygonG = new short[polygonId];
	short* tempPolygonB = new short[polygonId];
	bool* tempVertexEmbodyFlag = new bool[vertexId];
	bool* tempPolygonEmbodyFlag = new bool[polygonId];
	bool* tempPolygonInsideFlag = new bool[polygonId];

	Vector newGravityCenter;
	int embodyVertexNum = 0;
	for (int i = 0; i < vertexNum; i++) {
		if (i < vertexId) {
			tempVertex[i] = vertex[i];
			tempVertexEmbodyFlag[i] = vertexEmbodyFlag[i];
		} else {
			obj->vertex[i - vertexId] = vertex[i];//?? feiaj->fjeawo[i]
			obj->vertexEmbodyFlag[i - vertexId] = vertexEmbodyFlag[i];
			if (vertexEmbodyFlag[i]) {
				newGravityCenter += vertex[i];
				embodyVertexNum++;
			}
		}
	}
	obj->gravityCenter = newGravityCenter / embodyVertexNum;

	delete[] vertex;
	delete[] vertexEmbodyFlag;
	vertex = tempVertex;
	vertexEmbodyFlag = tempVertexEmbodyFlag;
	vertexNum = vertexId;

	for (int i = 0; i < lineNum; i++) {
		if (i < lineId) {
			tempLineLVertexIndex[i] = lineLVertexIndex[i];
			tempLineRVertexIndex[i] = lineRVertexIndex[i];
		} else {
			obj->lineLVertexIndex[i - lineId] = lineLVertexIndex[i] - vertexId;
			obj->lineRVertexIndex[i - lineId] = lineRVertexIndex[i] - vertexId;
		}
	}
	delete[] lineLVertexIndex;
	delete[] lineRVertexIndex;
	lineLVertexIndex = tempLineLVertexIndex;
	lineRVertexIndex = tempLineRVertexIndex;
	lineNum = lineId;

	for (short i = 0; i < polygonNum; i++) {
		if (i < polygonId) {
			tempPolygon1VertexIndex[i] = polygon1VertexIndex[i];
			tempPolygon2VertexIndex[i] = polygon2VertexIndex[i];
			tempPolygon3VertexIndex[i] = polygon3VertexIndex[i];
			tempPolygonR[i] = polygonR[i];
			tempPolygonG[i] = polygonG[i];
			tempPolygonB[i] = polygonB[i];
			tempPolygonEmbodyFlag[i] = polygonEmbodyFlag[i];
			tempPolygonInsideFlag[i] = polygonInsideFlag[i];
		} else {
			obj->polygon1VertexIndex[i - polygonId] = polygon1VertexIndex[i] - vertexId;
			obj->polygon2VertexIndex[i - polygonId] = polygon2VertexIndex[i] - vertexId;
			obj->polygon3VertexIndex[i - polygonId] = polygon3VertexIndex[i] - vertexId;
			obj->polygonR[i - polygonId] = polygonR[i];
			obj->polygonG[i - polygonId] = polygonG[i];
			obj->polygonB[i - polygonId] = polygonB[i];
			obj->polygonEmbodyFlag[i - polygonId] = polygonEmbodyFlag[i];
			obj->polygonInsideFlag[i - polygonId] = polygonInsideFlag[i];
		}
	}
	delete[] polygon1VertexIndex;
	delete[] polygon2VertexIndex;
	delete[] polygon3VertexIndex;
	delete[] polygonR;
	delete[] polygonG;
	delete[] polygonB;
	delete[] polygonEmbodyFlag;
	delete[] polygonInsideFlag;
	polygon1VertexIndex = tempPolygon1VertexIndex;
	polygon2VertexIndex = tempPolygon2VertexIndex;
	polygon3VertexIndex = tempPolygon3VertexIndex;
	polygonR = tempPolygonR;
	polygonG = tempPolygonG;
	polygonB = tempPolygonB;
	polygonEmbodyFlag = tempPolygonEmbodyFlag;
	polygonInsideFlag = tempPolygonInsideFlag;
	polygonNum = polygonId;

	return obj;
}

void Object::reloadRadius(void)
{
	for (short i = 0; i < vertexNum; i++) {
		if (vertexEmbodyFlag[i] == false)
			continue;
		if (radius < (vertex[i] - gravityCenter).getMagnitude())
			radius = (vertex[i] - gravityCenter).getMagnitude();
	}
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


const Vector& Object::getGravityCenter(void) const
{
	return gravityCenter;
}

const Vector& Object::getVertex(short vertexIndex) const
{
	return vertex[vertexIndex];
}

const Vector& Object::getPolygon1Vertex(short polygonIndex) const
{
	return vertex[polygon1VertexIndex[polygonIndex]];
}

const Vector& Object::getPolygon2Vertex(short polygonIndex) const
{
	return vertex[polygon2VertexIndex[polygonIndex]];
}

const Vector& Object::getPolygon3Vertex(short polygonIndex) const
{
	return vertex[polygon3VertexIndex[polygonIndex]];
}

const Vector& Object::getLineLVertex(short lineIndex) const
{
	return vertex[lineLVertexIndex[lineIndex]];
}

const Vector& Object::getLineRVertex(short lineIndex) const
{
	return vertex[lineRVertexIndex[lineIndex]];
}


Vector Object::getVrtxBasedOnG(short vrtxIdx)
{
	return vertex[vrtxIdx] - gravityCenter;
}

Vector Object::getPlgnBasedOnG(short plgnIdx)
{
//	here is more exact one.  however computer must calculate more and more.
	return (
		vertex[polygon1VertexIndex[plgnIdx]]
		+ vertex[polygon2VertexIndex[plgnIdx]]
		+ vertex[polygon3VertexIndex[plgnIdx]]
		) / 3.0 - gravityCenter;

//	approximately one
//	return vertex[polygon1VertexIndex[plgnIdx]] - gravityCenter;
}

Vector Object::getLineBasedOnG(short lineIdx)
{
//	here is more exact one.  however computer must calculate more and more.
	return (
		vertex[lineLVertexIndex[lineIdx]]
		+ vertex[lineRVertexIndex[lineIdx]]
		) / 2.0 - gravityCenter;

//	approximately one
//	return vertex[lineLVertexIndex[lineIdx]] - gravityCenter;
}

Vector Object::getDeltaVertex(short idx)
{
	Vector nextVertex = vertex[idx];
	Calculater::rotateRad(&nextVertex, gravityCenter, omegaVector, omega);

	return velocity + (nextVertex - vertex[idx]);
}

Vector Object::getDeltaPolygon(short idx)
{
	Vector R = getPlgnBasedOnG(idx);
	Vector nextR = R;
	Calculater::rotateRad(&nextR, Vector(0, 0, 0), omegaVector, omega);

	return velocity + (nextR - R);
}

Vector Object::getDeltaLine(short idx)
{
	Vector R = getLineBasedOnG(idx);
	Vector nextR = R;
	Calculater::rotateRad(&nextR, Vector(0, 0, 0), omegaVector, omega);

	return velocity + (nextR - R);
}

Vector Object::getPlgnInside(short idx)
{
	Vector inside = (
		(vertex[polygon2VertexIndex[idx]] - vertex[polygon1VertexIndex[idx]])
		% (vertex[polygon3VertexIndex[idx]] - vertex[polygon1VertexIndex[idx]])
		) * (isPolygonInside(idx) ? 1.0 : -1.0);

	return inside / inside.getMagnitude();
}

Array< Pair<int> > Object::getCommonVertexIdxInPolygons(short plgnIdx1, short plgnIdx2)
{
	Array< Pair<int> > pairList(3);

	for (short i = 0; i < 3; i++) {
		short vertexIdx1;
		switch (i) {
		case 0: vertexIdx1 = polygon1VertexIndex[plgnIdx1];	break;
		case 1: vertexIdx1 = polygon2VertexIndex[plgnIdx1];	break;
		case 2: vertexIdx1 = polygon3VertexIndex[plgnIdx1];	break;
		}

		for (short j = 0; j < 3; j++) {
			short vertexIdx2;
			switch (j) {
			case 0: vertexIdx2 = polygon1VertexIndex[plgnIdx2];	break;
			case 1: vertexIdx2 = polygon2VertexIndex[plgnIdx2];	break;
			case 2: vertexIdx2 = polygon3VertexIndex[plgnIdx2];	break;
			}

			if (vertexIdx1 == vertexIdx2) {
				pairList.add(Pair<int>(i + 1, j + 1));
				break;
			}
		}
	}

	return pairList;
}


float Object::getRadius(void) const
{
	return radius;
}

float Object::getMass(void)
{
	return mass;
}

float Object::getInertiaMoment(void)
{
	return inertiaMoment;
}

const Vector& Object::getVelocity(void) const
{
	return velocity;
}

Vector Object::getOmega(void) const
{
	return omegaVector * omega;
}

Vector Object::getOmegaBaseVector(void)
{
	return omegaVector;
}

float Object::getOmegaMagnitude(void)
{
	return omega;
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

ObjectStatus* Object::getStatus(void)
{
	return status;
}

const char* Object::getName(void)
{
	return name;
}

bool Object::isActive(void)
{
	return (velocity.getMagnitude() != 0);
}

bool Object::isFixed(void)
{
	return fixed;
}

bool Object::isVertexEmbody(short vertexIndex) const
{
	return vertexEmbodyFlag[vertexIndex];
}

bool Object::isPolygonEmbody(short polygonIndex) const
{
	return polygonEmbodyFlag[polygonIndex];
}

bool Object::isPolygonInside(short index) const
{
	return polygonInsideFlag[index];
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

void Object::setInertiaMoment(float inertiaMoment)
{
	this->inertiaMoment = inertiaMoment;
}

void Object::setDomination(bool originalIsDominated)
{
	isDominated = originalIsDominated;
}

void Object::fix(void)
{
	fixed = true;
	stop();
}

void Object::release(void)
{
	fixed = false;
}

void Object::update(void)
{
	this->run();
	this->rotate();
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
		Calculater::rotateRad(&vertex[i], gravityCenter, omegaVector, omega);
	}
}

void Object::push(Vector vector)
{
	if (fixed)
		return;
	velocity += (vector / mass);
}

void Object::accelerate(Vector vector)
{
	velocity += vector;
}
//fix check is executed in object class
void Object::applyTorque(Vector torque)
{
	if (fixed)
		return;
	omegaVector *= omega;
	omegaVector += (torque / inertiaMoment);
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