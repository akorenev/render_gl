#include "plypainter.h"

plypainter::plypainter()
    : m_plymodel(0)
    , m_vertex_i(0)
    , m_index_i(0)
    , m_drawFirst(false)
{

}

plypainter::~plypainter()
{
   // m_IFunctions->glDeleteBuffers(1, &m_vertex_i);
   // m_IFunctions->glDeleteBuffers(1, &m_index_i);
}

void plypainter::setModel(Model *model)
{
    m_plymodel = dynamic_cast<PlyModel*>(model);
    Q_ASSERT(m_plymodel);
    ipainter::setModel(model);
}

void plypainter::init_shaders()
{
    const char * vShaderStr =
            "#ifdef GL_ES \n"
            "precision mediump int;\n"
            "precision mediump float;\n"
            "#endif \n"
            "attribute vec4 vPosition; \n"
            "attribute vec4 vColor;\n"
            "uniform mat4 matrix; \n"
            "varying vec4 rColor;\n"
            "void main() \n"
            "{ \n"
            " gl_Position = matrix * vPosition; \n"
            " rColor = vColor; \n"
            "} \n";

    const char * fShaderStr =
            "#ifdef GL_ES \n"
            "precision mediump int;\n"
            "precision mediump float;\n"
            "#endif \n"
            "varying vec4 rColor;\n"
            "void main() \n"
            "{ \n"
            " gl_FragColor = rColor; \n"
            "} \n";

    ShaderInfoV shaderInfoV;
    ShaderInfo::Attributes attributesLocation;
    attributesLocation.push_back(Attribute("vPosition"));
    attributesLocation.push_back(Attribute("vColor"));
    ShaderInfo::Attributes attributesUniformLocation;
    attributesUniformLocation.push_back(Attribute("matrix"));
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(vShaderStr, ShaderInfo::VERTEX,
                                                         attributesLocation, attributesUniformLocation)));
    shaderInfoV.push_back(ShaderInfo::Ptr(new ShaderInfo(fShaderStr, ShaderInfo::FRAGMENT)));
    m_shader = Shader::Ptr(new Shader(m_IFunctions, shaderInfoV));
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

//    m_IFunctions->glEnableClientState(GL_VERTEX_ARRAY);

//    m_IFunctions->glBindBuffer(GL_ARRAY_BUFFER, m_vertex_i);
//    m_IFunctions->glVertexPointer(3, GL_DOUBLE, sizeof(double) * 3, 0);

//    m_IFunctions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_i);
//    m_IFunctions->glDrawElements(GL_TRIANGLES, faceVertexesSize, GL_UNSIGNED_INT, 0);

//    m_IFunctions->glDisableClientState(GL_VERTEX_ARRAY);

//    m_IFunctions->glBindBuffer(GL_ARRAY_BUFFER, 0);
//    m_IFunctions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_IFunctions->glUseProgram(m_shader->getProgramId());

    int idMatrix = m_shader->getShaderInfo()[0]->getKeyAttribute("matrix", ShaderInfo::UNIFORM);
    m_IFunctions->glUniformMatrix4fv(idMatrix, 1, 0, painterInfo->topMatrix().data());

    int idPosition = m_shader->getShaderInfo()[0]->getKeyAttribute("vPosition", ShaderInfo::LOCATION);


    m_IFunctions->glVertexAttribPointer(idPosition, 3, GL_FLOAT, GL_FALSE,
                                        sizeof(Vertex), &m_plymodel->getModel()->m_vertexes[0].point[0]);
    m_IFunctions->glEnableVertexAttribArray(idPosition);

    int idColor = m_shader->getShaderInfo()[0]->getKeyAttribute("vColor", ShaderInfo::LOCATION);
    m_IFunctions->glVertexAttribPointer(idColor, 4, GL_FLOAT, GL_TRUE,
                                        sizeof(Vertex), &m_plymodel->getModel()->m_vertexes[0].color[0]);
    m_IFunctions->glEnableVertexAttribArray(idColor);

    m_IFunctions->glDrawElements(GL_TRIANGLES, m_plymodel->getModel()->m_indices.size(),
                                 GL_UNSIGNED_SHORT,
                                 &m_plymodel->getModel()->m_indices[0]);

    m_IFunctions->glDisableVertexAttribArray(idPosition);

}

void plypainter::setIFunctions(IFunctions *iFunctions)
{
    ipainter::setIFunctions(iFunctions);
    init_shaders();
}
