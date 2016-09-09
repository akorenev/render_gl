#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include "item.h"

class ImageItem : public Item
{
public:
    ImageItem(unsigned char * bits,
              unsigned int depth,
              unsigned int w,
              unsigned int h);
    ~ImageItem();
    ImageItem(const ImageItem & other);
    const unsigned char * bits() const;
    unsigned int depth() const;
    Type getType() const;
    const BoundingBox& getBoundingBox() const;
    unsigned int w() const;
    unsigned int h() const;
    void setPos(const PointD & point);
    const PointD & getPos() const;
private:
    unsigned char * m_bits;
    unsigned int m_depth;
    unsigned int m_w;
    unsigned int m_h;
    PointD m_point;
};

#endif // IMAGEITEM_H
