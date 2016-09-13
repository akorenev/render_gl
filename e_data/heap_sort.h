#ifndef _HEAP_SORT_H_
#define _HEAP_SORT_H_

#include "swap.h"

//template<typename T>
//struct Value
//{
//    Value(T _value)
//        : value(_value){}
//    ~Value(){}
//    bool operator > (const Value & other)
//    {
//        return value > other.value;
//    }
//    T value;
//};

//template<typename T>
//std::ostream & operator << (std::ostream & os, const Value<T> & v)
//{
//    os << v.value;
//    return os;
//}

//    int m[] = {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
//    int size = sizeof(m) / sizeof(int);
//    sort_heap<int, int>(m, size);
//    for(int i = 0; i < size; ++i)
//        std::cout << " " << m[i];


template<typename Type>
Type left(Type i)
{
   return 2 * i + 1;
}

template<typename Type>
Type right(Type i)
{
    return 2 * i + 2;
}

template<typename Type>
Type parent(Type i)
{
    return i / 2;
}

template<typename T, typename Type>
void max_heapify(T * m, Type i, Type size)
{
    Type l = left(i);
    Type r = right(i);
    Type max_index = (l < size && m[l] > m[i]) ? l : i;
    if(r < size && m[r] > m[max_index])
        max_index = r;

    if(max_index != i)
    {
        swap_el<T>(&m[i], &m[max_index]);
        max_heapify<T, Type>(m, max_index, size);
    }
//     Type max_index;
//     bool f = false;
//     while(right(i)  < size && !f)
//     {
//         Type r = right(i);
//         if(r == size - 1 || m[r] > m[r + 1])
//             max_index = r;
//         else
//             max_index = r + 1;
//         if(m[i] < m[max_index])
//         {
//             swap_el<T>(&m[i], &m[max_index]);
//             i = max_index;
//         }
//         else
//         {
//             f = true;
//         }
//     }
}

template<typename T, typename Type>
void make_heap(T * m, Type size)
{
    int r = size / 2 - 1;
    for(int i = r; i > -1; --i)
        max_heapify<T, Type>(m, i, size);
}

template<typename T, typename Type>
void sort_heap(T * m, Type size)
{
    make_heap(m, size);
    for(int i = size - 1; i > 0; --i)
    {
        swap_el<T>(&m[0], &m[i]);
        max_heapify(m, 0 , i);
    }
}
#endif // _HEAP_SORT_H_
