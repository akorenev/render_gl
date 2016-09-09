#ifndef POINTITEM_H
#define POINTITEM_H

#include "item.h"

class PointItem : public Item
{
public:
    PointItem();
    Type getType() const;
    const PointD& getPoint() const;
    void setPoint(const PointD & point);
    void setFill(unsigned char fill[]);
    const unsigned char * getFill() const;
private:
    PointD m_point;
    unsigned char m_fill[4];

};

#endif // POINTITEM_H
