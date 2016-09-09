#ifndef PlyModel_H
#define PlyModel_H

#include "octotree.h"
#include "pointitem.h"

class PlyModel : public OctoModel
{
public:
    PlyModel(const BoundingBox & bbox);
    ~PlyModel();
    int getType();
    Item_t::Ptr searchFirstItem(const Ray & ray);
    int getVertexesSize();
    double * getVertexes();
    void setVertexes(double * vertexes, unsigned int vertexesSize);

    int getFaceVertexesSize();
    void setFaceVertexes(unsigned int * faceVertex, unsigned int faceVertexSize);
    unsigned int * getFaceVertexes();
private:
    void clearVertexes();
    void clearFaceVertexes();
    double * m_vertexes;
    unsigned int m_vertexesSize;
    unsigned int * m_faceVertex;
    unsigned int m_faceVertexSize;
};

#endif // PlyModel_H
