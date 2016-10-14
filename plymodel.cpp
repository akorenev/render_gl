#include "plymodel.h"

PlyModel::PlyModel(const BoundingBox &bbox)
    : OctoModel(4096, bbox)
    , m_model(0){}

PlyModel::~PlyModel()
{
}

int PlyModel::getType()
{
    return Model::PLY_MODEL;
}

Item_t::Ptr PlyModel::searchFirstItem(const Ray &ray)
{
    Item_t::Ptr item = OctoModel::searchFirstItem(ray);
    return item;
}

pointsitem * PlyModel::getModel()
{
    return m_model;
}

void PlyModel::setModel(pointsitem * model)
{
    m_model = model;
}
