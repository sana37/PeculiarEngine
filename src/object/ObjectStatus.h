#ifndef OBJECTSTATUS_H
#define OBJECTSTATUS_H

class ObjectStatus
{
	friend class Object;

private:
	ObjectStatus(void);
	ObjectStatus(const ObjectStatus&);

public:
	bool isFloat(void);
	void attach(void);
	void detach(void);

private:
	bool floating;

};

#endif
