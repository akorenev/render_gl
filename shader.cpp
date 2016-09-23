#include "shader.h"
#include <QDebug>

Shader::Shader()
    : m_ifunctions(0)
    , m_programId(0)
{}

Shader::Shader(IFunctions * ifunctions,
               const ShaderInfoV & shaderInfoV)
    : m_shaderInfoV(shaderInfoV)
    , m_ifunctions(ifunctions)
    , m_programId(0)
{
    init();
}

Shader::~Shader()
{
    ShaderInfoV::iterator it = m_shaderInfoV.begin();
    while(it != m_shaderInfoV.end())
    {
        m_ifunctions->glDeleteShader((*it)->shaderId);
        it = m_shaderInfoV.erase(it);
    }
    m_ifunctions->glDeleteProgram(m_programId);
}

void Shader::setShaderInfo(const ShaderInfoV &shaderInfoV)
{
    m_shaderInfoV = shaderInfoV;
}

const ShaderInfoV &Shader::getShaderInfo() const
{
    return m_shaderInfoV;
}

unsigned int Shader::getProgramId()
{
    return m_programId;
}

void Shader::init()
{
    for(auto & shaderInfo : m_shaderInfoV)
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
        shaderInfo->shaderId = shaderId;
        m_ifunctions->glShaderSource(shaderId, 1, &shaderInfo->value, NULL); // передать текст шейдера
        m_ifunctions->glCompileShader(shaderId); // скомпилировать шейдер

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
            qDebug() << QString(log);
        }
    }

    for(auto & shaderInfo : m_shaderInfoV)
    {
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
