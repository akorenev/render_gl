#ifndef SWAP_H
#define SWAP_H

template<typename T>
void swap_el(T * a, T * b)
{
    T t = *a;
    *a = *b;
    *b = t;
}

#endif // SWAP_H
