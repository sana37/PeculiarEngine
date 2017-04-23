#ifndef CRASHRESULT_H
#define CRASHRESULT_H

#include "CrashEvent.h"
#include "Vector.h"


class Field::CrashEvent::CrashResult
{
public:
	CrashResult(void);
	CrashResult(const CrashResult&);

	const CrashResult& operator=(const CrashResult&);

	void setResult(char);
	void setPlgnIdx(short);
	void setVrtxIdx(short);
	void setLine1Idx(short);
	void setLine2Idx(short);
	void setCrashSpot(Vector);

	char getResult(void);
	short getPlgnIdx(void);
	short getVrtxIdx(void);
	short getLine1Idx(void);
	short getLine2Idx(void);
	Vector getCrashSpot(void);

private:
	char result;
	short plgnIdx;
	short vrtxIdx;
	short line1Idx;
	short line2Idx;
	Vector crashSpot;
};

#endif
