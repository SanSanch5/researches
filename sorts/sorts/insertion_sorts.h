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
        while(pos >= p && arr[pos] > num)
		{
			arr[pos+1] = arr[pos];
			--pos;
		}	
		arr[pos+1] = num;	
	}
}

template<typename T>
int binarySearchPos(T *arr, int from, int end, const T &cmp)
{
    if(cmp >= arr[end]) return end+1;

    int pos = from;
    if(cmp > arr[from])
    {
        int right = end+1;
        int left = from;
        pos = (left+right)/2;
        while(arr[pos] < cmp || arr[pos-1] > cmp)
        {
            if(arr[pos] < cmp)
                left = pos;
            else
                right = pos;

            pos = (left + right) / 2;
        }
    }

    return pos;
}

template<typename T>
void binarySearchBlockedCopyInsertion(T *arr, int p, int r)
{
	for(int i = p+1; i <= r; ++i)
	{
		T num = arr[i];
        if(num >= arr[i-1]) continue;

        int pos = binarySearchPos(arr, p, i-1, num);
		memmove(arr+pos+1, arr+pos, (i-pos)*sizeof(T));
		arr[pos] = num;
	}
}

#endif
