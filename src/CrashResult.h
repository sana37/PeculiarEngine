#ifndef CRASHRESULT_H
#define CRASHRESULT_H

#include "CrashEvent.h"
#include "Vector.h"


class Field::CrashEvent::CrashResult
{
public:
	enum ResultCode {FAIL, POLYGON_AND_VERTEX, LINE_AND_LINE};

	CrashResult(void);
	CrashResult(const CrashResult&);
	virtual ~CrashResult(void);

	const CrashResult& operator=(const CrashResult&);

	void setObjPlgnAndVrtx(Object*, Object*, short, short);
	void setObjLineAndLine(Object*, Object*, short, short);
	void setResult(ResultCode);
	void setDist(float);
	void setCrashSpot(const Vector&);
	void setRelativeVelocity(const Vector&);

	Object* getObjPlgn(void);
	Object* getObjVrtx(void);
	Object* getObjLine1(void);
	Object* getObjLine2(void);
	ResultCode getResult(void);
	short getPlgnIdx(void);
	short getVrtxIdx(void);
	short getLine1Idx(void);
	short getLine2Idx(void);
	float getDist(void);
	Vector getCrashSpot(void);
	Vector getRelativeVelocity(void);

private:
	Object* objPlgnOrLine1;
	Object* objVrtxOrLine2;
	ResultCode result;
	short plgnIdx;
	short vrtxIdx;
	short line1Idx;
	short line2Idx;
	float dist;
	Vector crashSpot;
	Vector relativeVelocity;
};

#endif
