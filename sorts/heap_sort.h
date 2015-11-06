#ifndef HEAP_SORT
#define HEAP_SORT

#include <memory>

#include "heap.h"

template<typename T>
void heapSort(T *arr, int p, int r)
{
	Heap<T> heap(arr+p, r-p+1);
	heap.sort();
}

#endif
