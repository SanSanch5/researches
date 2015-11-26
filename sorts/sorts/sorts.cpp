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
#include "qsort_wrapper.h"
#include "tim_sort.h"
#include "tim_sort_not_mine.h"

using namespace::std;

typedef void (*Sort)(int *arr, int p, int r); 
typedef void (*SortVarN)(int *arr, int p, int r, int N);

bool testSort(Sort f, int *arr, int n, bool reverse = false)
{
    auto cmpFunc = [=](int a, int b) -> bool { return reverse ? a > b : a < b; };
	int *tmp = new int[n];
	copy(arr, arr+n, tmp);
	bool sorted = true;
	f(tmp, 0, n-1);
	for(int i = 1; (i < n); ++i)
        if(cmpFunc(tmp[i], tmp[i-1]))
			sorted = false;
    for(int i = 0; i < 10; ++i)
		cout << tmp[i] << " ";
	delete[] tmp;
	return sorted;
}

template<typename T>
void testSortings()
{
    int n = 100;
    int arr[100];
	cout << "\nTesting ..." << endl;
	for(int i = 0; i < n; ++i)
        arr[i] = rand() % 100;

	bool sorted = testSort(simpleInsertionSort<T>, arr, n);
	cout << "\t\t...Simple insertion " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(binarySearchBlockedCopyInsertion<T>, arr, n);
	cout << "\t\t...Binary search block memmoving insertion " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(merge_insertion<T>, arr, n);
	cout << "\t\t...Merge + insertion sort " << (sorted ? "OK" : "FAILED") << endl;	

    sorted = testSort(merge_insertion_galloped<T>, arr, n);
    cout << "\t\t...Merge sort galloped mode" << (sorted ? "OK" : "FAILED") << endl;

    sorted = testSort(heapSort<T>, arr, n);
    cout << "\t\t...Heap sort " << (sorted ? "OK" : "FAILED") << endl;

    sorted = testSort(reverseHeapSort<T>, arr, n, true);
    cout << "\t\t...Heap sort reverse" << (sorted ? "OK" : "FAILED") << endl;

	sorted = testSort(quickSort<T>, arr, n);
	cout << "\t\t...Quick sort " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(middleRandQuickSort<T>, arr, n);
	cout << "\t\t...Middle random quick sort " << (sorted ? "OK" : "FAILED") << endl;	

	sorted = testSort(smartPartitionQuickSort<T>, arr, n);
	cout << "\t\t...Smart partition ROR quick sort " << (sorted ? "OK" : "FAILED") << endl;	

    sorted = testSort(qsort_wrapper<T>, arr, n);
    cout << "\t\t...Wrapped Qt sort " << (sorted ? "OK" : "FAILED") << endl;

    sorted = testSort(timSort<T>, arr, n);
    cout << "\t\t...Timsort " << (sorted ? "OK" : "FAILED") << endl;

    sorted = testSort(timSortGalloped<T>, arr, n);
    cout << "\t\t...Galloped Timsort " << (sorted ? "OK" : "FAILED") << endl;

    sorted = testSort(timSortWithHeap<T>, arr, n);
    cout << "\t\t...Heap merging Timsort " << (sorted ? "OK" : "FAILED") << endl;

    sorted = testSort(mergeHeapSort<T>, arr, n);
    cout << "\t\t...Heap merging sort " << (sorted ? "OK" : "FAILED") << endl;

    sorted = testSort(timSortWrapper, arr, n);
    cout << "\t\t...Wrapped tim sort " << (sorted ? "OK" : "FAILED") << endl;
    cout << endl;
}

void printAnalyseResults(vector<int> vec, string start)
{
    cout << start << " N insertion better than merge = " << vec[MERGE] << endl;
    cout << start << " N insertion better than heap = " << vec[HEAP] << endl;
    cout << start << " N insertion better than random quick = " << vec[QUICK] << endl;
    cout << start << " N insertion better than median quick = " << vec[MEDIAN_QUICK] << endl;
    cout << start << " N insertion better than smart quick = " << vec[SMART_QUICK] << endl;
    cout << start << " N insertion better than merge heap = " << vec[MERGE_HEAP] << endl;
}

double getTimeOfSort(int *arr, int n, Sort sort, int iterations)
{
    int *tmp = new int[n];
    double res = 0;
    for(int i = 0; i < iterations; ++i)
    {
        copy(arr, arr+n, tmp);
        clock_t cl = clock();
        sort(tmp, 0, n-1);
        res += (double)(clock() - cl);
    }
    res /= CLOCKS_PER_SEC * iterations;
    delete[] tmp;
    return res;
}

void fillArray(int *arr, int N, int percent)
{
    int base = N * percent / 100;
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand();
        if(base != 0)
            arr[i] %= base;
    }
}

void getDataN(ofstream &outFile, int *arr, int N, int base,
              SortVarN sort, int limit, int iterations)
{
    fillArray(arr, N, base);
    int *tmp = new int[N];
    for(int i = 0; i <= limit; ++i)
    {
        cout << "Get time with " << i << " = low level" << endl;
        string time(to_string(i));
        clock_t cl = clock();

        cl = clock();
        for(int j = 0; j < iterations; ++j)
        {
            copy(arr, arr+N, tmp);
            sort(tmp, 0, N-1, i);
        }
        double t{(double)(clock() - cl) / CLOCKS_PER_SEC / iterations};
        time.append(" " + to_string(t));
        cout << "\ttime = " << t << endl;
        outFile << time << endl;
    }
    outFile.close();
    delete[] tmp;
}

int main(int argc, char **argv)
{
    if(argc == 2 && 0 == strcmp(argv[1], "--test"))
    {
        testSortings<int>();
        return 0;
    }
    else if(argc == 3 && 0 == strcmp(argv[1], "ar"))
	{
        if(0 == strcmp(argv[2], "-e"))
        {
            auto results = findInsertionEqualN();
            printAnalyseResults(results, "Max");
            return 0;
        }
        else if(0 == strcmp(argv[2], "-b"))
        {
            auto results = findInsertionBest();
            printAnalyseResults(results, "The best");
            return 0;
        }
	}

    else if(argc == 6 && strcmp(argv[1], "-n") == 0)
    {
        int N = atoi(argv[3]);
        int iterations = atoi(argv[4]);
        int *arr = new int[N];
        if(0 == strcmp(argv[5], "merge"))
        {
            ofstream outFileMerge (string(argv[2]) + "merge");
            getDataN(outFileMerge, arr, N, 0, merge_insertion_N_var<int>, 1200, iterations);
        }
        else if(0 == strcmp(argv[5], "median"))
        {
            ofstream outFileMedian(string(argv[2]) + "median");
            getDataN(outFileMedian, arr, N, 0, middleRandQuickSortVarN<int>, 730, iterations);
        }
        else if(0 == strcmp(argv[5], "quick"))
        {
            ofstream outFileQuick (string(argv[2]) + "quick");
            getDataN(outFileQuick, arr, N, 0, quickSortVarN<int>, 730, iterations);
        }
        else if(0 == strcmp(argv[5], "smart"))
        {
            ofstream outFileSmart (string(argv[2]) + "smart");
            getDataN(outFileSmart, arr, N, 1, smartPartitionQuickSortVarN<int>, 580, iterations);
        }
        else if(0 == strcmp(argv[5], "merge_heap"))
        {
            ofstream outFileSmart (string(argv[2]) + argv[5]);
            getDataN(outFileSmart, arr, N, 1, mergeHeapSortVarN<int>, 700, iterations);
        }
        return 0;
    }

	int N = 1000000;
    if(argc == 7 || argc == 8)
		N = atoi(argv[1]);
    else
    {
        cout << "\nUsage:" << endl;
        cout << "0. --test - run tests" << endl;
        cout << "1. <max_array_size> <iterations> <step> <base> <sorted/unsorted/partitionally/reverse> <destfile>" \
                 " [<partition percent>]" << endl;
        cout << "\twhen <step> shows array resizing stepfrom 0 to <max_array_size>, " \
                "<base> is a sparsity percent of random generated elements; base = 0 - max sparsity" << endl;
        cout << "2. ar - analise results\n\t-e - find N when each sort equals to insertion sort" << endl;
        cout << "\t-b - find what N in general should be used for the best results in each sort" << endl;
        cout << "\t\tNote, you need to hardcode results from 'ar -e' to func analyseInsertions" << endl;
        cout << "3. -n <destdir> <array_size> <iterations> <sort>, when <iterations> seems how many times" \
                " array must have been sorted for the clearest results\n" << endl;

        return 0;
    }

	int base = atoi(argv[4]);
	double time; 
	
	int iterations = atoi(argv[2]);
	int offset = atoi(argv[3]);
	ofstream outFile(argv[6]);	
	for(int n = 100; n < N; n += offset)
	{
        int *arr = new int[n];
        fillArray(arr, n, base);
        if(0 == strcmp(argv[5], "sorted"))
            heapSort(arr, 0, n-1);
        else if(0 == strcmp(argv[5], "partitionally"))
        {
//            int part = n / 10;
//            for(int partStart = 0; partStart + part < n; partStart += part)
//                heapSort(arr, partStart, partStart+part);
            int partitionPercent = atoi(argv[7]);
            heapSort(arr, 0, n-1);
            for(int sw = 0; sw < n*partitionPercent/200; ++sw)
                swap(arr[rand()%n], arr[rand()%n]);
        }
        else if(0 == strcmp(argv[5], "reverse"))
            reverseHeapSort(arr, 0, n-1);

        string times(to_string(n));
		cout << "Get time of sorting " << n << " elements array" << endl;	

//        time = getTimeOfSort(arr, n, merge_insertion_galloped<int>, iterations);
//        times.append(" " + to_string(time));

        time = getTimeOfSort(arr, n, merge_insertion<int>, iterations);
		times.append(" " + to_string(time));
//        cout << "Merge have got " << time << endl;
		 
        time = getTimeOfSort(arr, n, heapSort<int>, iterations);
		times.append(" " + to_string(time));
//        cout << "Heap have got " << time << endl;

        time = getTimeOfSort(arr, n, quickSort<int>, iterations);
		times.append(" " + to_string(time));
//        cout << "Quick have got " << time << endl;

        time = getTimeOfSort(arr, n, middleRandQuickSort<int>, iterations);
		times.append(" " + to_string(time));
//        cout << "Medial quick have got " << time << endl;

        time = getTimeOfSort(arr, n, smartPartitionQuickSort<int>, iterations);
		times.append(" " + to_string(time));
//        cout << "Smart partition quick have got " << time << endl;

        time = getTimeOfSort(arr, n, qsort_wrapper<int>, iterations);
        times.append(" " + to_string(time));
//        cout << "Included qsort have got " << time << endl;

//        time = getTimeOfSort(arr, n, timSort<int>, iterations);
//        times.append(" " + to_string(time));

        time = getTimeOfSort(arr, n, timSortGalloped<int>, iterations);
        times.append(" " + to_string(time));
//        cout << "Timsort have got " << time << endl;

        time = getTimeOfSort(arr, n, timSortWithHeap<int>, iterations);
        times.append(" " + to_string(time));
//        cout << "Timsort with heap merging have got " << time << endl;

        time = getTimeOfSort(arr, n, timSortWrapper, iterations);
        times.append(" " + to_string(time));

//        time = getTimeOfSort(arr, n, mergeHeapSort<int>, iterations);
//        times.append(" " + to_string(time));
        outFile << times << endl;
        delete[] arr;
	}
	cout << "Data for using in graphic can be found at " << argv[6] << endl;

	outFile.close();
	return 0;
}
