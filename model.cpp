#include "model.h"
#include "ray.h"

Model::Model(){}

Model::~Model()
{
    ItemUS::iterator it = m_itemUS.begin();
    while(it != m_itemUS.end())
    {
        delete *it;
        it = m_itemUS.erase(it);
    }
}

const ItemUS &Model::getItemL()
{
    return m_itemUS;
}

void Model::setItemL(const ItemUS &itemUS)
{
    m_itemUS = itemUS;
    for(auto & v : m_itemUS)
        BoundingBox::calculate(m_boundingBox, v->getBoundingBox());
}

const BoundingBox &Model::getBoundingBox()
{
    return m_boundingBox;
}

struct KeyMatchItem_t
{
    KeyMatchItem_t(Item_t::Ptr item)
        : m_item(item){}
    bool operator()(Item_t::Ptr item_t)
    {
        return item_t->item == m_item->item;
    }
private:
    Item_t::Ptr m_item;

};

void Model::searchItems(const Ray & ray,
                        Item_tL & item_tL)
{
    for(auto & i : m_itemUS)
    {
        PointDV v;
        const BoundingBox & bbox = i->getBoundingBox();
        v.push_back(bbox.min);
        v.push_back(bbox.max);
        if(Item_t::Ptr item = ray.intersected(v))
        {
            item->item = i;
            Item_tL::iterator it = std::find_if(item_tL.begin(), item_tL.end(), KeyMatchItem_t(item));
            if(it == item_tL.end())
                item_tL.push_back(item);
        }
    }
}

Item_t::Ptr Model::searchFirstItem(const Ray & ray)
{
    Item_tL item_tL;
    searchItems(ray, item_tL);
    if(item_tL.empty())
        return 0;
    item_tL.sort(SortItem_t);
    return *item_tL.begin();
}

int Model::getType()
{
    return NONE;
}
