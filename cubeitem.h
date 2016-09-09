#ifndef CUBEITEM_H
#define CUBEITEM_H

#include "item.h"
#include "imageitem.h"

class CubeItem : public Item
{
public:
    enum TYPE
    {
        BACK,
        RIGHT,
        LEFT,
        FRONT,
        BOTTOM,
        TOP
    };
    struct SIDE
    {
        SIDE()
            : m_texture(0){}
        SIDE(const PointD & one,
             const PointD & two,
             const PointD & three,
             const PointD & four,
             const ColorD color)
        {
            m_data[0] = one;
            m_data[1] = two;
            m_data[2] = three;
            m_data[3] = four;
            m_color = color;
            m_texture = 0;
        }
        SIDE(const SIDE & other)
        {
            copy_data(other);
            m_color = other.getColor();
            m_texture = new ImageItem(*other.getTexture());
        }

        SIDE & operator =(const SIDE & other)
        {
            if(&*this == &other)
                return *this;
            copy_data(other);
            m_color = other.getColor();
            ImageItem * imageItem = other.getTexture();
            if(imageItem)
                m_texture = new ImageItem(*imageItem);
            return *this;
        }

        void setColor(const ColorD & color)
        {
            m_color = color;
        }
        const ColorD & getColor() const
        {
            return m_color;
        }

        const PointD& getData(int i) const
        {
            return m_data[i];
        }

        void setData(const PointD & point, int i)
        {
            m_data[i] = point;
        }
        void setSize(const double & size)
        {
            m_size = size;
        }
        const double & getSize() const
        {
            return m_size;
        }
        ImageItem * getTexture() const
        {
            return m_texture;
        }

        void setTexture(ImageItem * texture) const
        {
            m_texture = texture;
        }

    private:
        PointD m_data[4];
        ColorD m_color;
        double m_size;
        mutable ImageItem * m_texture;
    private:
        void copy_data(const SIDE & side)
        {
            for(int i = 0; i < 4; ++i)
                m_data[i] = side.getData(i);
        }
    };
    CubeItem();
    CubeItem(const CubeItem & other);
    void setSide(const TYPE & type, const SIDE & side);
    const SIDE & getSide(const TYPE & type) const;
    Type getType() const;
    void setPos(const PointD & pos);
private:
    SIDE m_sides[6];
};

#endif // CUBEITEM_H
