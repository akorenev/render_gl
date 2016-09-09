#include "item.h"

Item::Item()
{

}

Item::~Item()
{

}

const BoundingBox & Item::getBoundingBox() const
{
    return m_boundingBox;
}

void Item::setPos(const PointD &pos)
{
    m_pos = pos;
}


