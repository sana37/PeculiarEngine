#ifndef OBJECT_H
#define OBJECT_H

#include "Vector.h"

class Object
{
public:
	Object(const char*);
	Object(const Object&);//copy constructer
	~Object(void);
	Object operator=(const Object&);
	void copyObject(const Object&);
	void composeObject(Object*);


	short getVertexNum(void) const;
	short getPolygonNum(void) const;
	short getLineNum(void) const;

	const Vector& getVertex(short) const;

	const Vector& getGravityCenter(void) const;

	const Vector& getPolygon1Vertex(short) const;
	const Vector& getPolygon2Vertex(short) const;
	const Vector& getPolygon3Vertex(short) const;

	const Vector& getLineLVertex(short) const;
	const Vector& getLineRVertex(short) const;

	float getRadius(void) const;

	const Vector& getVelocity(void) const;

	short getPolygonR(short) const;
	short getPolygonG(short) const;
	short getPolygonB(short) const;

	char whichClass(void) const;

	bool isActive(void);
	bool isVertexEmbody(short) const;
	bool isPolygonEmbody(short) const;


	void setVertex(short , const Vector&);
	void setVertex(short , float , float , float);
	void setGravityCenter(const Vector&);
	void setGravityCenter(float , float , float);
	void setVelocity(const Vector&);
	void setVelocity(float , float , float);

	void setDomination(bool);
	bool update(void);
	void run(void);
	void back(void);
	void stop(void);
	void moveRelative(const Vector&);
	void moveRelative(float , float , float);
	void moveAbsolute(const Vector&);
	void moveAbsolute(float , float , float);
	void enblack(short);


protected:
	char classCode;
	Vector velocity;
	bool isDominated;

private:
	short vertexNum;
	short lineNum;
	short polygonNum;

	Vector* vertex;
	Vector gravityCenter;

	short* lineLVertexIndex;
	short* lineRVertexIndex;

	short* polygon1VertexIndex;
	short* polygon2VertexIndex;
	short* polygon3VertexIndex;

	short* polygonR;
	short* polygonG;
	short* polygonB;

	float radius;
	bool* vertexEmbodyFlag;
	bool* polygonEmbodyFlag;

};

#endif
