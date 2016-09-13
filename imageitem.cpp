#include "imageitem.h"

ImageItem::ImageItem(unsigned char * bits,
                     unsigned int depth,
                     unsigned int w,
                     unsigned int h)
    : m_bits(bits)
    , m_depth(depth)
    , m_w(w)
    , m_h(h)
{
    setPos(m_point);
}

ImageItem::~ImageItem(){}

ImageItem::ImageItem(const ImageItem &other)
{
    m_depth = other.depth();
    m_w = other.w();
    m_h = other.h();
    m_boundingBox = other.getBoundingBox();
    unsigned int size = m_w * m_h * m_depth;
    m_bits = new unsigned char[size];
    std::fill(m_bits, m_bits + size, 0);
    std::copy(other.bits(), other.bits() + size, m_bits);
}

const unsigned char * ImageItem::bits() const
{
    return m_bits;
}

unsigned int ImageItem::depth() const
{
    return m_depth;
}

Item::Type ImageItem::getType() const
{
    return IMAGE;
}

const BoundingBox &ImageItem::getBoundingBox() const
{
    return m_boundingBox;
}

unsigned int ImageItem::w() const
{
    return m_w;
}

unsigned int ImageItem::h() const
{
    return m_h;
}

void ImageItem::setPos(const PointD &point)
{
    m_point = point;
    m_boundingBox.max[0] = m_point.x() + m_w;
    m_boundingBox.max[1] = m_point.y() + m_h;
    m_boundingBox.max[2] = m_point.z();
    m_boundingBox.min = m_point;
}

const PointD &ImageItem::getPos() const
{
    return m_point;
}

