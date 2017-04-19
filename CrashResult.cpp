#include "CrashResult.h"

Field::CrashEvent::CrashResult::CrashResult(void)
{
	result = false;
	distMin = 0;
}

Field::CrashEvent::CrashResult::CrashResult(const Field::CrashEvent::CrashResult& crashResult)
{
	result = crashResult.getResult();
	distMin = crashResult.getDistMin();
}

const Field::CrashEvent::CrashResult& Field::CrashEvent::CrashResult::operator=(const Field::CrashEvent::CrashResult& crashResult)
{
	result = crashResult.getResult();
	distMin = crashResult.getDistMin();

	return *this;
}

void Field::CrashEvent::CrashResult::setResult(char result)
{
	this->result = result;
}

void Field::CrashEvent::CrashResult::setDistMin(float distMin)
{
	this->distMin = distMin;
}

char Field::CrashEvent::CrashResult::getResult(void) const
{
	return result;
}

float Field::CrashEvent::CrashResult::getDistMin(void) const
{
	return distMin;
}
