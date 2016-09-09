#include "plymodel.h"

PlyModel::PlyModel(const BoundingBox &bbox)
    : OctoModel(4096, bbox)
    , m_vertexes(0)
    , m_vertexesSize(0)
    , m_faceVertexSize(0){}

PlyModel::~PlyModel()
{
    clearVertexes();
    clearFaceVertexes();
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

int PlyModel::getVertexesSize()
{
    return m_vertexesSize;
}

double * PlyModel::getVertexes()
{
    return m_vertexes;
}

void PlyModel::setVertexes(double *vertexes, unsigned int vertexesSize)
{
    clearVertexes();
    m_vertexes = vertexes;
    m_vertexesSize = vertexesSize;
}

void PlyModel::clearVertexes()
{
    if(m_vertexesSize)
    {
        delete [] m_vertexes;
        m_vertexesSize = 0;
    }
}

void PlyModel::clearFaceVertexes()
{
    if(m_faceVertexSize)
    {
        delete [] m_faceVertex;
        m_faceVertexSize = 0;
    }
}

int PlyModel::getFaceVertexesSize()
{
    return m_faceVertexSize;
}

void PlyModel::setFaceVertexes(unsigned int *faceVertex, unsigned int faceVertexSize)
{
    clearFaceVertexes();
    m_faceVertex = faceVertex;
    m_faceVertexSize = faceVertexSize;
}

unsigned int * PlyModel::getFaceVertexes()
{
    return m_faceVertex;
}
