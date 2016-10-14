#ifndef PLYPAINTER_H
#define PLYPAINTER_H

#include "ipainter.h"
#include "plymodel.h"
#include "shader.h"

class plypainter : public ipainter
{
public:
    plypainter();
    ~plypainter();
    void setModel(Model * model);
    void draw(IPainterInfo::Ptr painterInfo);
    void setIFunctions(IFunctions * iFunctions);
private:
    void init_shaders();
    PlyModel * m_plymodel;
    GLuint m_vertex_i;
    GLuint m_index_i;
    bool m_drawFirst;
    Shader::Ptr m_shader;
};

#endif // PLYPAINTER_H
