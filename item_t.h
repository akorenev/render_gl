#ifndef ITEM_T_H
#define ITEM_T_H

#include <QVector3D>
#include "item.h"

struct Item_t
{
    typedef std::shared_ptr<Item_t> Ptr;
    Item_t(){}
    Item_t(Item * _item, const QVector3D & _v, const double & _t)
        : item(_item)
        , v(_v)
        , t(_t){}
    Item * item;
    QVector3D v;
    double t;
};

typedef std::list<Item_t::Ptr> Item_tL;

inline bool SortItem_t(const Item_t::Ptr & first,
                       const Item_t::Ptr & second)
{
    return first->t < second->t;
}

#endif // ITEM_T_H
