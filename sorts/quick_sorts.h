#ifndef QUICK_SORTS
#define QUICK_SORTS

#include <memory>

#include "insertion_sorts.h"

template<typename T>
void swap(T *arr, int i, int j)
{
	if(i == j) return;
	std::swap(arr[i], arr[j]);
}

template<typename T>
void partition(T *arr, int p, int r, int &q)
{
	q = p-1;
	T x = arr[r];
	for(int j = p; j < r; ++j)
	{
		if(arr[j] <= x)
		{
			++q;
			swap<T>(arr, j, q);
		}	
	}
	++q;
	swap<T>(arr, r, q);
}

template<typename T>
void randPartition(T *arr, int p, int r, int &q)
{
	int ind = p + rand() % (r-p);
	swap<T>(arr, r, ind);
	partition<T>(arr, p, r, q);
}

template<typename T>
void quickSort(T *arr, int p, int r)
{
	if(r-p <= 63)
		binarySearchBlockedCopyInsertion<T>(arr, p, r);

	int q;
	while(p < r)
	{
		randPartition<T>(arr, p, r, q);
		quickSort<T>(arr, p, q-1);
		p = q+1;
	}
}

template<typename T>
void partitionSmart(T *arr, int p, int r, int &q, int &t)
{
	q = p-1;
	t = p-1;
	T x = arr[r];
	for(int j = p; j < r; ++j)
	{
		if(arr[j] <= x)
		{
			++t;
			if(arr[j] == x)
				swap<T>(arr, j, t);
			else
			{
				++q;
				swap<T>(arr, j, q);
				if(q != t)
					swap<T>(arr, t, j);
			}
		}	
	}
	++q;
	++t;
	swap<T>(arr, r, t);
}

template<typename T>
int middle(T *arr, int i1, int i2, int i3)
{
	int mid = i1;
	int min = i2;
	int max = i3;
	if(arr[min] > arr[max]) std::swap(min, max);
	if(arr[min] > arr[mid]) std::swap(min, mid);
	if(arr[mid] > arr[max]) std::swap(mid, max);	
	return mid;
}

template<typename T>
void randPartitionSmart(T *arr, int p, int r, int &q, int &t)
{
	int ind1 = p + rand() % (r-p);
	int ind2 = p + rand() % (r-p);
	int ind3 = p + rand() % (r-p);
	int mid = middle<T>(arr, ind1, ind2, ind3); 
	swap<T>(arr, r, mid);
	partitionSmart<T>(arr, p, r, q, t);
}

template<typename T>
void smartPartitionQuickSort(T *arr, int p, int r)
{
	if(r-p <= 63)
		binarySearchBlockedCopyInsertion<T>(arr, p, r);

	int q, t;
	while(p < r)
	{
		randPartitionSmart<T>(arr, p, r, q, t);
		smartPartitionQuickSort<T>(arr, p, q-1);
		p = t+1;
	}
}

template<typename T>
void middleRandPartition(T *arr, int p, int r, int &q)
{	
	int ind1 = p + rand() % (r-p);
	int ind2 = p + rand() % (r-p);
	int ind3 = p + rand() % (r-p);
	int mid = middle<T>(arr, ind1, ind2, ind3); 
	swap<T>(arr, r, mid);
	partition<T>(arr, p, r, q);
}

template<typename T>
void middleRandQuickSort(T *arr, int p, int r)
{
	if(r-p <= 63)
		binarySearchBlockedCopyInsertion<T>(arr, p, r);

	int q;
	while(p < r)
	{
		middleRandPartition<T>(arr, p, r, q);
		quickSort<T>(arr, p, q-1);
		p = q+1;
	}
}

#endif
