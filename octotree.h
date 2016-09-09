#ifndef OCTOTREE_H
#define OCTOTREE_H

#include "point.h"
#include "color.h"
#include "model.h"
#include "item.h"
#include "ray.h"

struct OctoTree
{
    enum Type
    {
        BRANCH,
        LEAF
    };
    OctoTree();
    ~OctoTree();
    void clear();
    PointD middle;
    double lenght;
    static int size;
    OctoTree * nodes[9];
    BoundingBox bbox;
    ItemUS items;
    Type type;

};

class OctoModel : public Model
{
public:
    OctoModel(int count, const BoundingBox & bbox);
    ~OctoModel();
    void setItemL(const ItemUS & items);
    void searchItems(const Ray & ray, Item_tL & item_tL);
    Item_t::Ptr searchFirstItem(const Ray & ray);
    const OctoTree * root() const;
    const BoundingBox & getBoundingBox();
    int getType();
private:
    void createNode(OctoTree ** el, int count, const PointD& middle,
                    double lenght, OctoTree * up, const BoundingBox & bbox);
    void delimSpace(OctoTree ** el, int count);
    void intersectBBox(const BoundingBox & bbox,
                       OctoTree * el,
                       std::list<OctoTree * > & nodeL);
    void clear();
    void search(OctoTree *el,
                ItemUS & itemUS,
                const Ray & ray);
    bool containsItem(Item * item, const ItemUS & itemL);

    void fillVector(Ray::Type type, PointDV & v, Item * item);

    OctoTree * m_root;
    int m_count;
};



#endif // OCTOTREE_H
