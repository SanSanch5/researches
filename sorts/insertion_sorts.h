#ifndef INSERTION_SORTS
#define INSERTION_SORTS

#include <cstring>
#include <memory>
#include <iostream>

template<typename T>
void simpleInsertionSort(T *arr, int p, int r)
{
	for(int i = p+1; i <= r; ++i)
	{
		T num = arr[i];
		int pos = i-1;
		while(pos >= 0 && arr[pos] > num)
		{
			arr[pos+1] = arr[pos];
			--pos;
		}	
		arr[pos+1] = num;	
	}
}

template<typename T>
void binarySearchBlockedCopyInsertion(T *arr, int p, int r)
{
	for(int i = p+1; i <= r; ++i)
	{
		T num = arr[i];
		if(num >= arr[i-1]) continue;

		int pos = 0;
		if(num > arr[0]) 
		{
			int right = i;
			int left = 0;
			pos = right/2;
			while(arr[pos] < num || arr[pos-1] > num)
			{
				if(arr[pos] < num)
					left = pos;
				else
					right = pos;

				pos = (left + right) / 2;
			}	
		}	

		memmove(arr+pos+1, arr+pos, (i-pos)*sizeof(T));
		arr[pos] = num;
	}
}

#endif
