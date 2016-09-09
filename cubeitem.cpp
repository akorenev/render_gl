#include "cubeitem.h"

CubeItem::CubeItem(){}

CubeItem::CubeItem(const CubeItem &other)
{
    for(int i = 0; i < 6; ++i)
        m_sides[i] = other.getSide(static_cast<CubeItem::TYPE>(i));
    m_pos.clear();
}

void CubeItem::setSide(const CubeItem::TYPE &type, const CubeItem::SIDE &side)
{
    for(int i = 0; i < 4; ++i)
    {
        const PointD & p = side.getData(i);
        m_boundingBox.min[0] = std::min(m_boundingBox.min.x(), p.x());
        m_boundingBox.min[1] = std::min(m_boundingBox.min.y(), p.y());
        m_boundingBox.min[2] = std::min(m_boundingBox.min.z(), p.z());

        m_boundingBox.max[0] = std::max(m_boundingBox.max.x(), p.x());
        m_boundingBox.max[1] = std::max(m_boundingBox.max.y(), p.y());
        m_boundingBox.max[2] = std::max(m_boundingBox.max.z(), p.z());
    }
    m_sides[type] = side;
}

const CubeItem::SIDE &CubeItem::getSide(const TYPE &type) const
{
    return m_sides[type];
}

Item::Type CubeItem::getType() const
{
    return CUBE;
}

void CubeItem::setPos(const PointD &pos)
{
    Item::setPos(pos);
    for(int i = 0; i < 6; ++i)
    {
        CubeItem::TYPE type = static_cast<CubeItem::TYPE>(i);
        const CubeItem::SIDE & side = getSide(type);
        CubeItem::SIDE value;
        value.setColor(side.getColor());
        for(int j = 0; j < 4; ++j)
        {
            const PointD & p = side.getData(j);
            value.setData(PointD(pos.x() + p.x(),
                                 pos.y() + p.y(),
                                 pos.z() + p.z()), j);
        }
        setSide(type, value);
    }
}
