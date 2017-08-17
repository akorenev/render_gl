#ifndef ITEM_H
#define ITEM_H

#include <list>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <memory>

#include "boundingbox.h"
#include "color.h"

#include <QVector3D>

class Item
{
public:
    enum Type
    {
        POLYGON,
        IMAGE,
        CUBE,
        POINT,
        TRIANGLE,
        SPHERE
    };
public:
    Item();
    virtual ~Item();
    virtual Type getType() const  = 0;
    const BoundingBox& getBoundingBox() const;
    virtual void setPos(const PointD & pos);
protected:
    BoundingBox m_boundingBox;
    PointD m_pos;
};

typedef std::unordered_set<Item *> ItemUS;

#endif // ITEM_H
