#ifndef CRASHRESULT_H
#define CRASHRESULT_H

#include "CrashEvent.h"

class Field::CrashEvent::CrashResult
{
public:
	CrashResult(void);
	CrashResult(const CrashResult&);

	const CrashResult& operator=(const CrashResult&);

	void setResult(char);
	void setDistMin(float);

	char getResult(void) const;
	float getDistMin(void) const;

private:
	char result;
	float distMin;
};

#endif
