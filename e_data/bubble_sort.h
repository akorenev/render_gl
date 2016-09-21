#ifndef _BUBBLE_SORT_H_
#define _BUBBLE_SORT_H_

#include "swap.h"
// O(n ^ 2)
template<typename T, typename Type>
void bubble_sort(T * m, Type size)
{
    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size - 1; ++j)
            if(m[j] > m[j + 1])
                swap_el<T>(&m[j], &m[j + 1]);
}

#endif // _BUBBLE_SORT_H_
