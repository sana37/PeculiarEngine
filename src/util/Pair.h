#ifndef PAIR_H
#define PAIR_H

#include <iostream>

template<typename T>
class Pair
{
public:
	Pair(void);
	Pair(T, T);
	Pair(const Pair<T>&);
	virtual ~Pair(void);

	void setInstance(T, T);
	void setInstance1(T);
	void setInstance2(T);

	T getInstance1(void);
	T getInstance2(void);

	const Pair<T>& operator=(const Pair<T>&);
	bool operator==(const Pair<T>&) const;

private:
	T instance1;
	T instance2;
};

template<typename T>
Pair<T>::Pair(void)
{
	instance1 = NULL;
	instance2 = NULL;
//	std::cerr << "make pair null\n";
}

template<typename T>
Pair<T>::Pair(T t1, T t2) : instance1(t1), instance2(t2)
{
//	std::cerr << "make pair ordinary\n";
}

template<typename T>
Pair<T>::Pair(const Pair<T>& pair)
{
	instance1 = pair.instance1;
	instance2 = pair.instance2;
//	std::cerr << " copy pair\n";
}

template<typename T>
Pair<T>::~Pair(void)
{
//do nothing?
//	std::cerr << "delete pair\n";
}

template<typename T>
void Pair<T>::setInstance(T t1, T t2)
{
	instance1 = t1;
	instance2 = t2;
}

template<typename T>
void Pair<T>::setInstance1(T t)
{
	instance1 = t;
}

template<typename T>
void Pair<T>::setInstance2(T t)
{
	instance2 = t;
}

template<typename T>
T Pair<T>::getInstance1(void)
{
	return instance1;
}

template<typename T>
T Pair<T>::getInstance2(void)
{
	return instance2;
}

template<typename T>
const Pair<T>& Pair<T>::operator=(const Pair<T>& pair)
{
	instance1 = pair.instance1;
	instance2 = pair.instance2;

	return *this;
}

template<typename T>
bool Pair<T>::operator==(const Pair<T>& pair) const
{
	if ((instance1 == pair.instance1)  &&  (instance2 == pair.instance2))
		return true;

	if ((instance1 == pair.instance2)  &&  (instance2 == pair.instance1))
		return true;

	return false;
}

#endif
