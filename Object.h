#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"

class Object
{
public:
	Object(const char*);
	Object(const Object&);//copy constructer
	~Object(void);
	void inherit(bool* , short* , short* , short* , short* , short*) const;
	Object operator=(const Object&);
	void copyObject(const Object&);
	void composeObject(Object*);


	short getVertexNum(void) const;
	short getPolygonNum(void) const;
	short getLineNum(void) const;

	void getVertex(short , Vector*) const;
	const Vector& getVertex(short) const;

	void getGravityCenter(Vector*) const;
	const Vector& getGravityCenter(void) const;

	void getPolygon1Vertex(short , Vector*) const;
	void getPolygon2Vertex(short , Vector*) const;
	void getPolygon3Vertex(short , Vector*) const;
	const Vector& getPolygon1Vertex(short) const;
	const Vector& getPolygon2Vertex(short) const;
	const Vector& getPolygon3Vertex(short) const;

	void getLineLVertex(short , Vector*) const;
	void getLineRVertex(short , Vector*) const;
	const Vector& getLineLVertex(short) const;
	const Vector& getLineRVertex(short) const;

	float getRadius(void) const;

	void getVelocity(Vector*) const;
	const Vector& getVelocity(void) const;

	short getPolygonR(short) const;
	short getPolygonG(short) const;
	short getPolygonB(short) const;

	char whichClass(void) const;

	bool isActive(void);
	bool isVertexEmbody(short) const;
	bool isPolygonEmbody(short) const;


	void setVertex(short , Vector*);
	void setGravityCenter(Vector*);
	void setVelocity(Vector*);

	void setDomination(bool);
	bool update(void);
	void move(void);
	void back(void);
	void stop(void);
	void transport(Vector*);
	void teleport(Vector*);
	void enblack(short);


protected:
	char classCode;
	Vector* velocity;
	bool isDominated;

private:
	short vertexNum;
	short polygonNum;
	short lineNum;

	Vector** vertex;
	Vector* gravityCenter;

	float radius;
	bool* vertexEmbodyFlag;
	bool* polygonEmbodyFlag;

	short* polygon1VertexIndex;
	short* polygon2VertexIndex;
	short* polygon3VertexIndex;

	short* lineLVertexIndex;
	short* lineRVertexIndex;

	short* polygonR;
	short* polygonG;
	short* polygonB;

};

#endif
