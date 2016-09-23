#include "plypainter.h"

plypainter::plypainter()
    : m_plymodel(0)
    , m_vertex_i(0)
    , m_index_i(0)
    , m_drawFirst(false){}

plypainter::~plypainter()
{
    m_IFunctions->glDeleteBuffers(1, &m_vertex_i);
    m_IFunctions->glDeleteBuffers(1, &m_index_i);
}

void plypainter::setModel(Model *model)
{
    m_plymodel = dynamic_cast<PlyModel*>(model);
    Q_ASSERT(m_plymodel);
    ipainter::setModel(model);
}

void plypainter::draw(IPainterInfo::Ptr painterInfo)
{
//    double * vertexes = m_plymodel->getVertexes();
//    unsigned int vertexesSize = m_plymodel->getVertexesSize();
//    if(!vertexesSize)
//        return;
//    unsigned int * faceVertexes = m_plymodel->getFaceVertexes();
//    unsigned int faceVertexesSize = m_plymodel->getFaceVertexesSize();
//    m_IFunctions->glColor3f(1.0, 1.0, 1.0);
//    if(!m_drawFirst)
//    {
//        m_IFunctions->glGenBuffers(1, &m_vertex_i);
//        m_IFunctions->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_i);
//        m_IFunctions->glBufferData(GL_ARRAY_BUFFER, sizeof(double) * 3 * vertexesSize, vertexes, GL_STATIC_DRAW);

//        m_IFunctions->glGenBuffers(1, &m_index_i);
//        m_IFunctions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_i);
//        m_IFunctions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* faceVertexesSize, faceVertexes, GL_STATIC_DRAW);
//        m_drawFirst = true;
//    }

//    // m_IFunctions.glEnableClientState(GL_VERTEX_ARRAY);
//    // m_IFunctions.glVertexPointer(3, GL_DOUBLE, sizeof(double) * 3, vertexes);
//    // m_IFunctions.glPointSize(2.0);
//    // m_IFunctions.glColor3f(0, 1.0, 0);
//    // m_IFunctions.glDrawElements(GL_TRIANGLES, faceVertexesSize, GL_UNSIGNED_INT, &faceVertexes[0]);
//    // //m_IFunctions.glDrawArrays(GL_POINTS, 0, vertexesSize / 3);
//    // m_IFunctions.glDisableClientState(GL_VERTEX_ARRAY);

//    m_IFunctions->glEnableClientState(GL_VERTEX_ARRAY);

//    m_IFunctions->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_i);
//    m_IFunctions->glVertexPointer(3, GL_DOUBLE, sizeof(double) * 3, 0);

//    m_IFunctions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_i);
//    m_IFunctions->glDrawElements(GL_TRIANGLES, faceVertexesSize, GL_UNSIGNED_INT, 0);

//    m_IFunctions->glDisableClientState(GL_VERTEX_ARRAY);

//    m_IFunctions->glBindBuffer(GL_ARRAY_BUFFER, 0);
//    m_IFunctions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
