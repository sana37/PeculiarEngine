#include "CrashResult.h"
#include "Vector.h"

Field::CrashEvent::CrashResult::CrashResult(void) : result(false), plgnIdx(-1), vrtxIdx(-1), line1Idx(-1), line2Idx(-1), crashSpot()
{
//	distMin = 0;
}

Field::CrashEvent::CrashResult::CrashResult(const Field::CrashEvent::CrashResult& crashResult)
{
	result = crashResult.result;
	plgnIdx = crashResult.plgnIdx;
	vrtxIdx = crashResult.vrtxIdx;
	line1Idx = crashResult.line1Idx;
	line2Idx = crashResult.line2Idx;
	crashSpot = crashResult.crashSpot;
//	distMin = crashResult.getDistMin();
}

const Field::CrashEvent::CrashResult& Field::CrashEvent::CrashResult::operator=(const Field::CrashEvent::CrashResult& crashResult)
{
	result = crashResult.result;
	plgnIdx = crashResult.plgnIdx;
	vrtxIdx = crashResult.vrtxIdx;
	line1Idx = crashResult.line1Idx;
	line2Idx = crashResult.line2Idx;
	crashSpot = crashResult.crashSpot;
//	distMin = crashResult.getDistMin();

	return *this;
}

void Field::CrashEvent::CrashResult::setResult(char result)
{
	this->result = result;
}

void Field::CrashEvent::CrashResult::setPlgnIdx(short idx)
{
	plgnIdx = idx;
}

void Field::CrashEvent::CrashResult::setVrtxIdx(short idx)
{
	vrtxIdx = idx;
}

void Field::CrashEvent::CrashResult::setLine1Idx(short idx)
{
	line1Idx = idx;
}

void Field::CrashEvent::CrashResult::setLine2Idx(short idx)
{
	line2Idx = idx;
}

void Field::CrashEvent::CrashResult::setCrashSpot(Vector vector)
{
	crashSpot = vector;
}

/*
void Field::CrashEvent::CrashResult::setDistMin(float distMin)
{
	this->distMin = distMin;
}
*/

char Field::CrashEvent::CrashResult::getResult(void)
{
	return result;
}

short Field::CrashEvent::CrashResult::getPlgnIdx(void)
{
	return plgnIdx;
}

short Field::CrashEvent::CrashResult::getVrtxIdx(void)
{
	return vrtxIdx;
}

short Field::CrashEvent::CrashResult::getLine1Idx(void)
{
	return line1Idx;
}

short Field::CrashEvent::CrashResult::getLine2Idx(void)
{
	return line2Idx;
}

Vector Field::CrashEvent::CrashResult::getCrashSpot(void)
{
	return crashSpot;
}

/*
float Field::CrashEvent::CrashResult::getDistMin(void)
{
	return distMin;
}
*/