#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

// O(n ^ 2)
template<typename T, typename Type>
void insertion_sort(T * m, Type size)
{
    for(Type i = 1; i < size; ++i)
    {
        T key = m[i];
        Type j = i - 1;
        while (j > -1 && m[j] > key)
        {
            m[j + 1] = m[j];
            j -= 1;
        }
        m[j + 1] = key;
    }
}

#endif // INSERTION_SORT_H
