#ifndef ARRAY_H
#define ARRAY_H

#define INITIAL_MAX	50

#include <iostream>

template<typename T>
class Array
{
public:
	Array(void);
	explicit Array(int);
	explicit Array(const Array<T>&);
	virtual ~Array(void);

	T operator[](int);
	T get(int);
	void set(int, T);
	void add(T);
	void remove(int);
	int length(void);

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
	std::cerr << "Array: start " << max << "\n";
}

template<typename T>
Array<T>::Array(int initialMax)
{
	if (initialMax <= 0)
		initialMax = INITIAL_MAX;
	array = new T[initialMax];
	max = initialMax;
	size = 0;
	std::cerr << "Array: start " << max << "\n";
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
}

template<typename T>
Array<T>::~Array(void)
{
	delete[] array;
	std::cerr << "Array: size " << size << "\n";
}

template<typename T>
T Array<T>::operator[](int idx)
{
	if (idx < size) {
		return array[idx];
	} else {
		std::cerr << "Array: invalid access\n";
		return 0;
	}
}

template<typename T>
T Array<T>::get(int idx)
{
	if (idx < size) {
		return array[idx];
	} else {
		std::cerr << "Array: invalid access\n";
		return 0;
	}
}

template<typename T>
void Array<T>::set(int idx, T t)
{
	if (idx < size) {
		array[idx] = t;
	} else {
		std::cerr << "Array: invalid access\n";
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
//	delete array[idx];//??!!

	size--;
	for (short i = idx  ;  i < size  ;  i++) {
		array[i] = array[i + 1];
	}
}

template<typename T>
int Array<T>::length(void)
{
	return size;
}


#endif
