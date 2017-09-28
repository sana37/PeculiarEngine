#ifndef ARRAY_H
#define ARRAY_H

#include "Define.h"
#include <iostream>

template<typename T>
class Array
{
public:
	Array(void);
/*	explicit*/ Array(int);
	Array(const Array<T>&);
	virtual ~Array(void);

	T operator[](int) const;
	T get(int);
	void set(int, T);
	void add(T);
	void remove(int);
	void removeAll(void);
	void removeIfMatchOnce(T);
	void removeIfMatchAll(T);//if int == T ??
	void removeDuplicated(void);
	bool has(T);
	int length(void) const;

private:
	T* array;
	int max;
	int size;
};


template<typename T>
Array<T>::Array(void)
{
	array = new T[INITIAL_MAX];
	max = INITIAL_MAX;
	size = 0;
//	std::cerr << "Array: start " << max << "\n";
}

template<typename T>
Array<T>::Array(int initialMax)
{
	if (initialMax <= 0)
		initialMax = INITIAL_MAX;
	array = new T[initialMax];
	max = initialMax;
	size = 0;
//	std::cerr << "Array: start " << max << "\n";
}

template<typename T>
Array<T>::Array(const Array<T>& ary)
{
	max = ary.max;
	size = ary.size;
	array = new T[max];

	for (int i = 0  ;  i < size  ;  i++) {
		array[i] = ary.array[i];
	}
//	std::cerr << "Array:   copy " << max << "\n";
}

template<typename T>
Array<T>::~Array(void)
{
	delete[] array;
//	if (size != max  &&  size != 0)
//		std::cerr << "Array: size " << size << " , max " << max << "\n";
}

template<typename T>
T Array<T>::operator[](int idx) const
{
	if (idx < size) {
		return array[idx];
	} else {
		std::cerr << "Array: invalid access\n";
		std::terminate();
	}
}

template<typename T>
T Array<T>::get(int idx)
{
	if (idx < size) {
		return array[idx];
	} else {
		std::cerr << "Array: invalid access\n";
		std::terminate();
	}
}

template<typename T>
void Array<T>::set(int idx, T t)
{
	if (idx < size) {
		array[idx] = t;
	} else {
		std::cerr << "Array: invalid access\n";
		std::terminate();
	}
}

template<typename T>
void Array<T>::add(T t)
{
	if (size == max) {
		std::cerr << "extend!!\n";
		max *= 2;
		T* newArray = new T[max];

		for (int i = 0  ;  i < size  ;  i++) {
			newArray[i] = array[i];
		}

		delete[] array;
		array = newArray;
	}

	array[size] = t;
	size++;
}

template<typename T>
void Array<T>::remove(int idx)
{
	if (idx < 0  ||  size <= idx)
		return;

	size--;
	for (int i = idx  ;  i < size  ;  i++) {
		array[i] = array[i + 1];
	}
}

template<typename T>
void Array<T>::removeAll(void)
{
	size = 0;
}

template<typename T>
void Array<T>::removeIfMatchOnce(T t)
{
	for (int i = 0  ;  i < size  ;  i++) {
		if (array[i] == t) {
			remove(i);
			return;
		}
	}
}

template<typename T>
void Array<T>::removeIfMatchAll(T t)
{
	for (int i = 0  ;  i < size  ;  i++) {
		if (array[i] == t) {
			remove(i);
			i--;
		}
	}
}

template<typename T>
void Array<T>::removeDuplicated(void)
{
	for (short i = 0  ;  i < size  ;  i++) {
		for (short j = i + 1  ;  j < size  ;  j++) {
			if (array[i] == array[j]) {
				remove(j);
				j--;
			}
		}
	}
}

template<typename T>
bool Array<T>::has(T t)
{
	for (int i = 0; i < size; i++) {
		if (array[i] == t)
			return true;
	}

	return false;
}

template<typename T>
int Array<T>::length(void) const
{
	return size;
}

#endif
