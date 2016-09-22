#ifndef PLYPAINTER_H
#define PLYPAINTER_H

#include "ipainter.h"
#include "plymodel.h"

class plypainter : public ipainter
{
public:
    plypainter();
    ~plypainter();
    void setModel(Model * model);
    void draw(IPainterInfo::Ptr painterInfo);
private:
    PlyModel * m_plymodel;
    GLuint m_vertex_i;
    GLuint m_index_i;
    bool m_drawFirst;
};

#endif // PLYPAINTER_H
