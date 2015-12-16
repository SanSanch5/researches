#ifndef TIM_SORT_H
#define TIM_SORT_H

#include "insertion_sorts.h"
#include "merge_sort.h"
#include "heap.h"

struct MinArr
{
    int start;
    int n;
    int current;

    MinArr() {}
    MinArr(int _s, int _n) : current(0), start(_s), n(_n) {}
    ~MinArr() {}

    MinArr &operator =(const MinArr &_arr)
    {
        start = _arr.start;
        n = _arr.n;
        current = _arr.current;

        return *this;
    }
};

struct MinArrays
{
    int n;
    MinArr *arrs;

    void push(MinArr arr)
    {
        arrs[n] = arr;
        ++n;
    }

    MinArr pop()
    {
        --n;
        return arrs[n];
    }

    MinArr & operator [](int i)
    {
        return arrs[i];
    }
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
MinArrays getMinArrs(T *arr, int n, int mr)
{
    MinArrays res;
    res.n = 0;
    res.arrs = new MinArr[n/mr + 1];

    int k {1};
    while(k < n)
    {
        MinArr ma(k-1, 0);
        auto direction = [](T a, T b) -> bool { return a <= b; };
        bool curDirection = direction(arr[k-1], arr[k]);
        while(k < n && ++k && curDirection == direction(arr[k-1], arr[k]));
        if(!curDirection)
            revert(arr + ma.start, k - ma.start);

        ma.n = k - ma.start;
        ma.n = ma.n < mr
                ? (k + mr < n ? ma.n + mr : n - ma.start)
                : ma.n;
        res.push(ma);
        k = ma.start + ma.n + 1;
    }
    if(k == n)
        res.push(MinArr(k-1, 1));

    return res;
}

template<typename T>
void stackStep(T *arr, MinArrays &st)
{
    if(st.n <= 2) return;

    MinArr z = st.pop();
    MinArr y = st.pop();
    MinArr x = st.pop();

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
void mergeArrs(T *arr, MinArrays &minArrs)
{
    MinArrays st;
    st.n = 0;
    st.arrs = new MinArr[minArrs.n];

    for(int i = 0; i < minArrs.n; ++i)
    {
        st.push(minArrs[i]);
        stackStep(arr, st);
    }

    while(st.n != 1)
    {
        MinArr y = st.pop();
        MinArr x = st.pop();
        merge(arr, x.start, x.start + x.n - 1, y.start + y.n - 1);
        x.n += y.n;
        st.push(x);
    }

    delete[] st.arrs;
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

    for(int i = 0; i < arrs.n; ++i)
        binarySearchBlockedCopyInsertion(arr, arrs[i].start, arrs[i].start + arrs[i].n-1);

    mergeArrs(arr, arrs);

    delete[] arrs.arrs;
}

template<typename T>
struct ElementAndWhereItFrom
{
    T elem;
    int arrayFrom;

    ElementAndWhereItFrom() {}
    ElementAndWhereItFrom(const T &_elem, int _arrayFrom) : elem(_elem), arrayFrom(_arrayFrom) {}

    ElementAndWhereItFrom &operator =(const ElementAndWhereItFrom &_e)
    {
        elem = _e.elem;
        arrayFrom = _e.arrayFrom;
        return *this;
    }

    bool operator <(const ElementAndWhereItFrom &_el) const
    {
        return elem < _el.elem;
    }

    bool operator >(const ElementAndWhereItFrom &_el) const
    {
        return elem > _el.elem;
    }
};

    // my modification
template<typename T>
void heapMergeArrs(T *arr, MinArrays &minArrs)
{
    int n = minArrs[minArrs.n - 1].start + minArrs[minArrs.n - 1].n;
    T *merged = new T[n];
    std::memcpy(merged, arr, sizeof(T) * n);

    ElementAndWhereItFrom<T> *tmp = new ElementAndWhereItFrom<T>[minArrs.n];
    Heap<ElementAndWhereItFrom<T>> heap(tmp, 0, Heap<ElementAndWhereItFrom<T>>::MIN);
    for(int i = 0; i < minArrs.n; ++i)
        heap.insert(ElementAndWhereItFrom<T>(merged[minArrs[i].start], i)), ++minArrs[i].current;

    int k(0);
    while(heap.size() != 0)
    {
        ElementAndWhereItFrom<T> min = heap.top();
        arr[k] = min.elem;
        MinArr &cur = minArrs[min.arrayFrom];
        if(cur.current < cur.n)
        {
            heap.updateKey(0, ElementAndWhereItFrom<T>(merged[cur.start+cur.current], min.arrayFrom));
            heap.heapify(0);
            ++cur.current;
        }
        else
            heap.pop();

        ++k;
    }

    delete[] merged;
    delete[] tmp;
}

    // my modification
template<typename T>
void timSortWithHeap(T *arr, int p, int r)
{
    int n = r-p+1;
    int mr = getMinRun(n);
    if(n < mr)
    {
        binarySearchBlockedCopyInsertion(arr, p, r);
        return;
    }

    auto arrs = getMinArrs(arr+p, n, mr);

    for(int i = 0; i < arrs.n; ++i)
        binarySearchBlockedCopyInsertion(arr, arrs[i].start, arrs[i].start + arrs[i].n-1);

    heapMergeArrs(arr, arrs);
}

template<typename T>
MinArrays getMinArrsSimple(T *arr, int n)
{
    MinArrays res;
    res.n = 0;
    res.arrs = new MinArr[n/2 + 1];

    int k {1};
    while(k < n)
    {
        MinArr ma(k-1, 0);
        auto direction = [](T a, T b) -> bool { return a <= b; };
        bool curDirection = direction(arr[k-1], arr[k]);
        while(k < n && ++k && curDirection == direction(arr[k-1], arr[k]));
        if(!curDirection)
            revert(arr + ma.start, k - ma.start);

        ma.n = k - ma.start;
        res.push(ma);
        k = ma.start + ma.n + 1;
    }
    if(k == n)
        res.push(MinArr(k-1, 1));

    return res;
}

 // my sort
template<typename T>
void mergeHeapSort(T *arr, int p, int r)
{
    int n = r-p+1;
    auto arrs = getMinArrsSimple(arr+p, n);

    if(arrs.n != 1)
        heapMergeArrs(arr, arrs);

    delete[] arrs.arrs;
}

template<typename T>
void mergeHeapSortVarN(T *arr, int p, int r, int N = 0)
{
    if(r-p+1 <= N)
    {
        binarySearchBlockedCopyInsertion(arr, p, r);
        return;
    }

    int n = r-p+1;
    auto arrs = getMinArrs(arr+p, n, N);
    for(int i = 0; i < arrs.n; ++i)
        binarySearchBlockedCopyInsertion(arr, arrs[i].start, arrs[i].start + arrs[i].n-1);

    heapMergeArrs(arr, arrs);

    delete[] arrs.arrs;
}

template<typename T>
int nextMinArr(MinArrays &dest, T *arr, int n, int mr, int from)
{
    ++from;
    if(from > n) return 0;

    MinArr ma(from-1, 0);
    auto direction = [](T a, T b) -> bool { return a <= b; };
    bool curDirection = direction(arr[from-1], arr[from]);
    while(from < n && ++from && curDirection == direction(arr[from-1], arr[from]));
    if(!curDirection)
        revert(arr + ma.start, from - ma.start);

    ma.n = from - ma.start;
    bool needSort = ma.n < mr;
    ma.n = needSort
            ? (from + mr < n ? ma.n + mr : n - ma.start)
            : ma.n;
    dest.push(ma);
    from = ma.start + ma.n;
    if(needSort)
        binarySearchBlockedCopyInsertion(arr, ma.start, from-1);

    return from;
}

template<typename T>
void stackStepGalloped(T *arr, MinArrays &st)
{
    if(st.n <= 2) return;

    int xs = st.arrs[st.n-1].n;
    int ys = st.arrs[st.n-2].n;
    int zs = st.arrs[st.n-3].n;
    if(!(xs > ys + zs && ys > zs))
    {
        MinArr z = st.pop();

        if(zs < xs)
        {
            MinArr &y = st.arrs[st.n-1];
            gallopedMerge(arr, y.start, y.start + y.n - 1, z.start + z.n-1);
            y.n += z.n;
        }
        else
        {
            MinArr &y = st.arrs[st.n-1];
            MinArr &x = st.arrs[st.n-2];
            gallopedMerge(arr, x.start, x.start + x.n - 1, y.start + y.n-1);
            x.n += y.n;
            y = z;
        }
        stackStepGalloped(arr, st);
    }
}

template<typename T>
void gallopedMergeArrs(T *arr, int n, int mr)
{
    MinArrays st;
    st.n = 0;
    st.arrs = new MinArr[n/mr + 1];

    int from{0};
    while(from = nextMinArr(st, arr, n, mr, from))
        stackStepGalloped(arr, st);

    while(st.n != 1)
    {
        MinArr y = st.pop();
        MinArr &x = st.arrs[st.n-1];
        gallopedMerge(arr, x.start, x.start + x.n - 1, y.start + y.n - 1);
        x.n += y.n;
    }

    delete[] st.arrs;
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

    gallopedMergeArrs(arr, r-p+1, mr);
}

#endif // TIM_SORT_H
