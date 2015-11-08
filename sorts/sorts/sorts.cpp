#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <time.h>
#include <cstring>

#include "insertion_sorts.h"
#include "quick_sorts.h"
#include "heap_sort.h"
#include "merge_sort.h"
#include "resultsanalyser.h"

using namespace::std;

typedef void (*Sort)(int *arr, int p, int r); 

bool testSort(Sort f, int *arr, int n)
{
	int *tmp = new int[n];
	copy(arr, arr+n, tmp);
	bool sorted = true;
	f(tmp, 0, n-1);
	for(int i = 1; (i < n); ++i)
		if(tmp[i] < tmp[i-1]) 
			sorted = false;
	for(int i = 0; i < 10; ++i)
		cout << tmp[i] << " ";
	delete[] tmp;
	return sorted;
}

template<typename T>
void testSortings()
{
	int n = 300;
	int arr[300];
	cout << "\nTesting ..." << endl;
	for(int i = 0; i < n; ++i)
		arr[i] = rand() % 1000;

	bool sorted = testSort(simpleInsertionSort<T>, arr, n);
	cout << "\t\t...Simple insertion " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(binarySearchBlockedCopyInsertion<T>, arr, n);
	cout << "\t\t...Binary search block memmoving insertion " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(merge_insertion<T>, arr, n);
	cout << "\t\t...Merge + insertion sort " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(heapSort<T>, arr, n);
	cout << "\t\t...Heap sort " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(quickSort<T>, arr, n);
	cout << "\t\t...Quick sort " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(middleRandQuickSort<T>, arr, n);
	cout << "\t\t...Middle random quick sort " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(smartPartitionQuickSort<T>, arr, n);
	cout << "\t\t...Smart partition ROR quick sort " << (sorted ? "OK" : "FAILED") << endl;	
	cout << endl;
}

void printAnalyseResults(vector<int> vec)
{
	cout << "Max N insertion better than merge = " << vec[MERGE] << endl;
	cout << "Max N insertion better than heap = " << vec[HEAP] << endl;
	cout << "Max N insertion better than random quick = " << vec[QUICK] << endl;
	cout << "Max N insertion better than median quick = " << vec[MEDIAN_QUICK] << endl;
	cout << "Max N insertion better than smart quick = " << vec[SMART_QUICK] << endl;
}

int main(int argc, char **argv)
{
	testSortings<int>();

	if(argc == 2 && 0 == strcmp(argv[1], "ar"))
	{
        auto results = findInsertionEqualN();
		printAnalyseResults(results);
		return 0;
	}

    else if(argc == 3 && strcmp(argv[1], "-n") == 0)
    {
        ofstream outFile(argv[2]);
        int N = 50000;
        int *arr = new int[N];
        int *tmp = new int[N];
        for (int i = 0; i < N; i++)
            arr[i] = rand() % 1000;
        for(int i = 30; i < 50000; i += 1000)
        {
            cout << "Get time with " << i << " = low level" << endl;
            string time(to_string(i));
            clock_t cl = clock();

            cl = clock();
            for(int i = 0; i < 1000; ++i)
            {
                copy(arr, arr+N, tmp);
                merge_insertion_N_var<int>(tmp, 0, N-1, i);
            }
            time.append(" " + to_string((double)(clock() - cl) / CLOCKS_PER_SEC / 1000));
            outFile << time << endl;
        }

        return 0;
    }

	int N = 1000000;
	if(argc >= 6)
		N = atoi(argv[1]);
	else return 0;

	int *arr = new int[N];
	int base = atoi(argv[4]);
	for (int i = 0; i < N; i++)
	{
		arr[i] = rand();
		if(base != 0)
			arr[i] %= base;
	}
	if(0 == strcmp(argv[5], "sorted"))
		heapSort(arr, 0, N-1);	
	
	clock_t cl = clock();
	int *tmp = new int[N];
	double time; 
	
	int iterations = atoi(argv[2]);
	int offset = atoi(argv[3]);
	ofstream outFile(argv[6]);	
	for(int n = 100; n < N; n += offset)
	{
		string times(to_string(n));
		cout << "Get time of sorting " << n << " elements array" << endl;	
		cl = clock();
		for(int i = 0; i < iterations; ++i)
		{
			copy(arr, arr+n, tmp);
			binarySearchBlockedCopyInsertion<int>(tmp, 0, n-1);
		}
		time = (double)(clock() - cl) / CLOCKS_PER_SEC / iterations;
		times.append(" " + to_string(time));
// 		cout << "Binary search block moving insertion elapsed time " << time << endl;

		cl = clock();
		for(int i = 0; i < iterations; ++i)
		{
			copy(arr, arr+n, tmp);
			merge_insertion<int>(tmp, 0, n-1);
		}
		time = (double)(clock() - cl) / CLOCKS_PER_SEC / iterations;
		times.append(" " + to_string(time));
		 
		cl = clock();
		for(int i = 0; i < iterations; ++i)
		{
			copy(arr, arr+n, tmp);
			heapSort(tmp, 0, n-1);
		}
		time = (double)(clock() - cl) / CLOCKS_PER_SEC / iterations;
		times.append(" " + to_string(time));
		 
		cl = clock();
		for(int i = 0; i < iterations; ++i)
		{
			copy(arr, arr+n, tmp);
			quickSort(tmp, 0, n-1);
		}
		time = (double)(clock() - cl) / CLOCKS_PER_SEC / iterations;
		times.append(" " + to_string(time));
// 		cout << "Randomized optimal recursive (ROR) quick sort elapsed time " << time << endl;
	 
		cl = clock();
		for(int i = 0; i < iterations; ++i)
		{
			copy(arr, arr+n, tmp);
			middleRandQuickSort(tmp, 0, n-1);
		}
		time = (double)(clock() - cl) / CLOCKS_PER_SEC / iterations;
		times.append(" " + to_string(time));
// 		cout << "Middle random quick sort elapsed time " << time << endl;
	 
		cl = clock();
		for(int i = 0; i < iterations; ++i)
		{
			copy(arr, arr+n, tmp);
			smartPartitionQuickSort(tmp, 0, n-1);
		}
		time = (double)(clock() - cl) / CLOCKS_PER_SEC / iterations;
		times.append(" " + to_string(time));
// 		cout << "Smart partition ROR quick sort elapsed time " << time << endl;
// 		cout << endl;
		outFile << times << endl;
	}
	cout << "Data for using in graphic can be found at " << argv[6] << endl;

	outFile.close();
	delete[] arr;
	delete[] tmp;
	return 0;
}
