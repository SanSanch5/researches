#ifndef MERGE_SORT
#define MERGE_SORT

#include <memory>

#include "insertion_sorts.h"

template<typename T>
inline void debugPrint(T *arr, int p, int r)
{
	for(int i = p; i <= r; ++i)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
}

template<typename T>
void merge(T *arr, int p, int mid, int r)
{
	int n = mid-p+1;
	T *tmp = new T[n];
	std::memmove(tmp, arr+p, sizeof(T)*n);
	
	int k{0};
	int i{p};
	int j(mid+1);
	
	while(k < n)
	{

		if(j > r)
		{
			std::memmove(arr+i, tmp+k, sizeof(T)*(n-k));
			break;
		}
		else if(arr[j] < tmp[k])
		{
			arr[i] = arr[j];
			++j;
		}	
		else
		{
			arr[i] = tmp[k];
			++k;
		}
		++i;
	}

	delete[] tmp;	
}

template<typename T>
void merge_insertion(T *arr, int p, int r)
{
	if(r-p <= 0) 
	{
        if(r-p <= 840)
            binarySearchBlockedCopyInsertion<T>(arr, p, r);
		return;
	}

	int mid = (p+r)/2;
	merge_insertion(arr, p, mid);
	merge_insertion(arr, mid+1, r);
	merge(arr, p, mid, r);	
}

template<typename T>
void merge_insertion_N_var(T *arr, int p, int r, int N = 0)
{
    if(r-p <= 0)
    {
        if(r-p <= N)
            binarySearchBlockedCopyInsertion<T>(arr, p, r);
        return;
    }

    int mid = (p+r)/2;
    merge_insertion(arr, p, mid);
    merge_insertion(arr, mid+1, r);
    merge(arr, p, mid, r);
}

#endif
