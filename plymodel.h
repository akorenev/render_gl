#ifndef PlyModel_H
#define PlyModel_H

#include "octotree.h"
#include "pointsitem.h"

class PlyModel : public OctoModel
{
public:
    PlyModel(const BoundingBox & bbox);
    ~PlyModel();
    int getType();
    Item_t::Ptr searchFirstItem(const Ray & ray);
    pointsitem * getModel();
    void setModel(pointsitem * model);
private:
    pointsitem * m_model;
};

#endif // PlyModel_H
