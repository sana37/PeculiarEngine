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

	void setObjPlgn(Object*);
	void setObjVrtx(Object*);
	void setObjLine1(Object*);
	void setObjLine2(Object*);
	void setResult(ResultCode);
	void setPlgnIdx(short);
	void setVrtxIdx(short);
	void setLine1Idx(short);
	void setLine2Idx(short);
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
