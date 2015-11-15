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
void gallopedMerge(T *arr, int p, int mid, int r)
{
    int n = mid-p+1;
    T *tmp = new T[n];
    std::memcpy(tmp, arr+p, sizeof(T)*n);

    int k{0}; // pos in tmp array
    int i{p}; // pos for insertion next element
    int j(mid+1); // current pos in second-part array
    int contFrom1 = 0;
    int contFrom2 = 0;

    while(k < n)
    {
        if(contFrom1 == 7)
        {
            int pos = binarySearchPos(tmp, k, n-1, arr[j]);
            int n_move = pos - k;
            std::memcpy(arr+i, tmp+k, sizeof(T)*n_move);
            i += n_move, k = pos;
            contFrom1 = 0, contFrom2 = 0;

            if(k >= n) break;
        }
        else if(contFrom2 == 7)
        {
            int pos = binarySearchPos(arr, j, r, tmp[k]);
            int n_move = pos - j;
            std::memcpy(arr+i, arr+j, sizeof(T)*n_move);
            i += n_move, j = pos;
            contFrom1 = 0, contFrom2 = 0;
        }
        if(j > r)
        {
            std::memmove(arr+i, tmp+k, sizeof(T)*(n-k));
            break;
        }
        else if(arr[j] < tmp[k])
        {
            arr[i] = arr[j];
            ++j, ++contFrom2, contFrom1 = 0;
        }
        else
        {
            arr[i] = tmp[k];
            ++k, ++contFrom1, contFrom2 = 0;
        }
        ++i;
    }

    delete[] tmp;
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
    if(r-p <= 32)
    {
        binarySearchBlockedCopyInsertion<T>(arr, p, r);
        return;
    }

    int mid = (p+r)/2;
    merge_insertion(arr, p, mid);
    merge_insertion(arr, mid+1, r);
    merge(arr, p, mid, r);
}

template<typename T>
void merge_insertion_galloped(T *arr, int p, int r)
{
    if(r-p <= 1200)
	{
        binarySearchBlockedCopyInsertion<T>(arr, p, r);
		return;
	}

	int mid = (p+r)/2;
    merge_insertion_galloped(arr, p, mid);
    merge_insertion_galloped(arr, mid+1, r);
    gallopedMerge(arr, p, mid, r);
}

template<typename T>
void merge_insertion_N_var(T *arr, int p, int r, int N = 0)
{
    if(r-p <= N)
    {
        binarySearchBlockedCopyInsertion<T>(arr, p, r);
        return;
    }

    int mid = (p+r)/2;
    merge_insertion_N_var(arr, p, mid, N);
    merge_insertion_N_var(arr, mid+1, r, N);
    merge(arr, p, mid, r);
}

#endif
