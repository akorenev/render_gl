#include "shader.h"
#include <QDebug>

Shader::Shader()
    : m_ifunctions(0)
    , m_programId(0)
{}

Shader::Shader(IFunctions * ifunctions,
               const ShaderInfoL & shaderInfoL)
    : m_shaderInfoL(shaderInfoL)
    , m_ifunctions(ifunctions)
    , m_programId(0)
{
    init();
}

Shader::~Shader()
{
    ShaderInfoL::iterator it = m_shaderInfoL.begin();
    while(it != m_shaderInfoL.end())
    {
        m_ifunctions->glDeleteShader((*it)->shaderId);
        it = m_shaderInfoL.erase(it);
    }
    m_ifunctions->glDeleteProgram(m_programId);
}

void Shader::setShaderInfo(const ShaderInfoL &shaderInfoL)
{
    m_shaderInfoL = shaderInfoL;
}

const ShaderInfoL &Shader::getShaderInfo() const
{
    return m_shaderInfoL;
}

unsigned int Shader::getProgramId()
{
    return m_programId;
}

void Shader::init()
{
    for(auto & shaderInfo : m_shaderInfoL)
    {
        unsigned int shaderId = 0;
        switch(shaderInfo->type)
        {
        case ShaderInfo::VERTEX:
            shaderId = m_ifunctions->glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderInfo::FRAGMENT:
            shaderId = m_ifunctions->glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            continue;
        }
        m_ifunctions->glShaderSource(shaderId, 1, &shaderInfo->value, NULL); // передать текст шейдера
        m_ifunctions->glCompileShader(shaderId); // скомпилировать шейдер
        //m_ifunctions->glGetCo
        if(m_programId == 0)
            m_programId = m_ifunctions->glCreateProgram();

        m_ifunctions->glAttachShader(m_programId, shaderId);
        m_ifunctions->glLinkProgram(m_programId);

        int ok;
        m_ifunctions->glGetProgramiv(m_programId, GL_LINK_STATUS, &ok); // проверить сборку
        if(!ok)
        {
            GLchar log[32768];
            GLsizei length;
            m_ifunctions->glGetShaderInfoLog(shaderId, 32768, &length, log );
            m_ifunctions->glDeleteShader(shaderId);
            qDebug() << "error attach Shader " << m_ifunctions->glGetError();
            qDebug() << QString(log);
            continue;
        }
        for(ShaderInfo::Attributes::iterator it = shaderInfo->attributes.begin();
            it != shaderInfo->attributes.end(); ++it)
        {
            const char * name = it->first;
            int id = m_ifunctions->glGetUniformLocation(m_programId, name); // проверить сущ. аттрибута
            if(id == -1)
            {
                qDebug() << "could not bind attribute " << name;
            }
            else
            {
                it->second = id;
            }
        }
    }
}
