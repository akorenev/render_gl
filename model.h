#ifndef _MODEL_H_
#define _MODEL_H_

#include "ray.h"

class Model
{
public:
    enum Type
    {
        NONE,
        OCTOTREE,
        PLY_MODEL
    };
public:
    Model(bool needRemoved = true);
    virtual ~Model();
    virtual const ItemUS& getItemL();
    virtual void setItemL(const ItemUS & itemUS);
    virtual const BoundingBox & getBoundingBox();
    virtual void searchItems(const Ray & ray, Item_tL & item_tL);
    virtual Item_t::Ptr searchFirstItem(const Ray & ray);
    virtual int getType();
protected:
    ItemUS m_itemUS;
    BoundingBox m_boundingBox;
    bool m_needRemoved;
};

typedef std::list<Model *> ModelL;


template <typename T>
T cmerge(const T&  a, const T & b)
{
    T t(a);
    t.insert(b.begin(), b.end());
    return t;
}

#endif // _MODEL_H_
