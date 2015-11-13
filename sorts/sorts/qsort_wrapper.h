#ifndef QSORT_WRAPPER_H
#define QSORT_WRAPPER_H

#include <QtAlgorithms>

template<typename T>
void qsort_wrapper(T *arr, int p, int r)
{
    qsort(arr+p, r-p+1, sizeof(T), [=](const void *a, const void *b) -> int
    {
        const T *ta = static_cast<const T *>(a);
        const T *tb = static_cast<const T *>(b);
        return (int)(*ta - *tb);
    });
}

#endif // QSORT_WRAPPER_H
