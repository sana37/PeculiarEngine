#include "CrashResult.h"

Field::CrashEvent::CrashResult::CrashResult(void) :
	objPlgnOrLine1(NULL),
	objVrtxOrLine2(NULL),
	result(FAIL),
	plgnIdx(-1),
	vrtxIdx(-1),
	line1Idx(-1),
	line2Idx(-1),
	dist(1.0),
	crashSpot(),
	relativeVelocity()
{
}

Field::CrashEvent::CrashResult::CrashResult(const Field::CrashEvent::CrashResult& crashResult)
{
	objPlgnOrLine1 = crashResult.objPlgnOrLine1;
	objVrtxOrLine2 = crashResult.objVrtxOrLine2;
	result = crashResult.result;
	plgnIdx = crashResult.plgnIdx;
	vrtxIdx = crashResult.vrtxIdx;
	line1Idx = crashResult.line1Idx;
	line2Idx = crashResult.line2Idx;
	dist = crashResult.dist;
	crashSpot = crashResult.crashSpot;
	relativeVelocity = crashResult.relativeVelocity;
}

Field::CrashEvent::CrashResult::~CrashResult(void)
{
	objPlgnOrLine1 = NULL;
	objVrtxOrLine2 = NULL;
}

const Field::CrashEvent::CrashResult& Field::CrashEvent::CrashResult::operator=(const Field::CrashEvent::CrashResult& crashResult)
{
	objPlgnOrLine1 = crashResult.objPlgnOrLine1;
	objVrtxOrLine2 = crashResult.objVrtxOrLine2;
	result = crashResult.result;
	plgnIdx = crashResult.plgnIdx;
	vrtxIdx = crashResult.vrtxIdx;
	line1Idx = crashResult.line1Idx;
	line2Idx = crashResult.line2Idx;
	dist = crashResult.dist;
	crashSpot = crashResult.crashSpot;
	relativeVelocity = crashResult.relativeVelocity;

	return *this;
}

void Field::CrashEvent::CrashResult::setObjPlgn(Object* obj)
{
	objPlgnOrLine1 = obj;
}

void Field::CrashEvent::CrashResult::setObjVrtx(Object* obj)
{
	objVrtxOrLine2 = obj;
}

void Field::CrashEvent::CrashResult::setObjLine1(Object* obj)
{
	objPlgnOrLine1 = obj;
}

void Field::CrashEvent::CrashResult::setObjLine2(Object* obj)
{
	objVrtxOrLine2 = obj;
}

void Field::CrashEvent::CrashResult::setResult(ResultCode result)
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

void Field::CrashEvent::CrashResult::setDist(float dist)
{
	this->dist = dist;
}

void Field::CrashEvent::CrashResult::setCrashSpot(const Vector& vector)
{
	crashSpot = vector;
}

void Field::CrashEvent::CrashResult::setRelativeVelocity(const Vector& vector)
{
	relativeVelocity = vector;
}


Object* Field::CrashEvent::CrashResult::getObjPlgn(void)
{
	if (result == LINE_AND_LINE)
		return NULL;
	else
		return objPlgnOrLine1;
}

Object* Field::CrashEvent::CrashResult::getObjVrtx(void)
{
	if (result == LINE_AND_LINE)
		return NULL;
	else
		return objVrtxOrLine2;
}

Object* Field::CrashEvent::CrashResult::getObjLine1(void)
{
	if (result == POLYGON_AND_VERTEX)
		return NULL;
	else
		return objPlgnOrLine1;
}

Object* Field::CrashEvent::CrashResult::getObjLine2(void)
{
	if (result == POLYGON_AND_VERTEX)
		return NULL;
	else
		return objVrtxOrLine2;
}

Field::CrashEvent::CrashResult::ResultCode Field::CrashEvent::CrashResult::getResult(void)
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

float Field::CrashEvent::CrashResult::getDist(void)
{
	return dist;
}

Vector Field::CrashEvent::CrashResult::getCrashSpot(void)
{
	return crashSpot;
}

Vector Field::CrashEvent::CrashResult::getRelativeVelocity(void)
{
	return relativeVelocity;
}
