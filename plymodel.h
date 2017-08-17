#ifndef PlyModel_H
#define PlyModel_H

#include "octotree.h"
#include "triangleitem.h"

class PlyModel : public OctoModel
{
public:
    PlyModel(const BoundingBox & bbox);
    ~PlyModel();
    int getType();
    Item_t::Ptr searchFirstItem(const Ray & ray);
    TriangleItem * getModel();
    void setModel(TriangleItem * model);
private:
    TriangleItem * m_model;
};

#endif // PlyModel_H
