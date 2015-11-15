#ifndef TIM_SORT_H
#define TIM_SORT_H

#include <stack>
#include <vector>
#include "insertion_sorts.h"
#include "merge_sort.h"

struct MinArr
{
    int start;
    int n;

    MinArr(int _s, int _n) : start(_s), n(_n) {}
    ~MinArr() {}
};

int getMinRun(int N)
{
    int r {0};

    while(N >= 64)
    {
        r |= N & 1;
        N >>= 1;
    }

    return N + r;
}

template<typename T>
void revert(T *arr, int n)
{
    for(int i = 0; i < n/2; ++i)
        std::swap(arr[i], arr[n-i-1]);
}

template<typename T>
std::vector<MinArr> getMinArrs(T *arr, int n, int mr)
{
    std::vector<MinArr> res;

    int k {1};
    while(k < n)
    {
        MinArr ma(k-1, 0);
        auto direction = [=](T a, T b) -> bool { return a <= b; };
        bool curDirection = direction(arr[k-1], arr[k]);
        while(k < n && ++k && curDirection == direction(arr[k-1], arr[k]));
        if(!curDirection)
            revert(arr + ma.start, k - ma.start);

        ma.n = k - ma.start;
        ma.n = ma.n < mr
                ? (k + mr < n ? ma.n + mr : n - ma.start)
                : ma.n;
        res.push_back(ma);
        k = ma.start + ma.n + 1;
    }

    return res;
}

template<typename T>
void stackStep(T *arr, std::stack<MinArr> &st)
{
    if(st.size() == 1 || st.size() == 2) return;

    MinArr z = st.top(); st.pop();
    MinArr y = st.top(); st.pop();
    MinArr x = st.top(); st.pop();

    if(!(x.n > y.n + z.n && y.n > z.n))
    {
        if(z.n < x.n)
        {
            merge(arr, y.start, y.start + y.n - 1, z.start + z.n-1);
            y.n += z.n;
            st.push(x), st.push(y);
        }
        else
        {
            merge(arr, x.start, x.start + x.n - 1, y.start + y.n-1);
            x.n += y.n;
            st.push(x), st.push(z);
        }
        stackStep(arr, st);
    }
    else
        st.push(x), st.push(y), st.push(z);
}


template<typename T>
void mergeArrs(T *arr, std::vector<MinArr> &minArrs)
{
    std::stack<MinArr> st;

    for(auto &a : minArrs)
    {
        st.push(a);
        stackStep(arr, st);
    }

    while(st.size() != 1)
    {
        MinArr y = st.top(); st.pop();
        MinArr x = st.top(); st.pop();
        merge(arr, x.start, x.start + x.n - 1, y.start + y.n - 1);
        x.n += y.n;
        st.push(x);
    }
}

template<typename T>
void timSort(T *arr, int p, int r)
{
    int n = r-p+1;
    int mr = getMinRun(n);
    if(n < mr)
    {
        binarySearchBlockedCopyInsertion(arr, p, r);
        return;
    }

    auto arrs = getMinArrs(arr+p, n, mr);

    for(MinArr &subArr : arrs)
        binarySearchBlockedCopyInsertion(arr, subArr.start, subArr.start + subArr.n-1);

    mergeArrs(arr, arrs);
}


template<typename T>
void stackStepGalloped(T *arr, std::stack<MinArr> &st)
{
    if(st.size() == 1 || st.size() == 2) return;

    MinArr z = st.top(); st.pop();
    MinArr y = st.top(); st.pop();
    MinArr x = st.top(); st.pop();

    if(!(x.n > y.n + z.n && y.n > z.n))
    {
        if(z.n < x.n)
        {
            gallopedMerge(arr, y.start, y.start + y.n - 1, z.start + z.n-1);
            y.n += z.n;
            st.push(x), st.push(y);
        }
        else
        {
            gallopedMerge(arr, x.start, x.start + x.n - 1, y.start + y.n-1);
            x.n += y.n;
            st.push(x), st.push(z);
        }
        stackStep(arr, st);
    }
    else
        st.push(x), st.push(y), st.push(z);
}


template<typename T>
void gallopedMergeArrs(T *arr, std::vector<MinArr> &minArrs)
{
    std::stack<MinArr> st;

    for(auto &a : minArrs)
    {
        st.push(a);
        stackStepGalloped(arr, st);
    }

    while(st.size() != 1)
    {
        MinArr y = st.top(); st.pop();
        MinArr x = st.top(); st.pop();
        gallopedMerge(arr, x.start, x.start + x.n - 1, y.start + y.n - 1);
        x.n += y.n;
        st.push(x);
    }
}

template<typename T>
void timSortGalloped(T *arr, int p, int r)
{
    int n = r-p+1;
    int mr = getMinRun(n);
    if(n < mr)
    {
        binarySearchBlockedCopyInsertion(arr, p, r);
        return;
    }

    auto arrs = getMinArrs(arr+p, n, mr);

    for(MinArr &subArr : arrs)
        binarySearchBlockedCopyInsertion(arr, subArr.start, subArr.start + subArr.n-1);

    gallopedMergeArrs(arr, arrs);
}

#endif // TIM_SORT_H
