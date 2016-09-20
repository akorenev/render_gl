#include "shader.h"
#include <QDebug>

Shader::Shader()
    : m_value(0)
    , m_type(NONE)
    , m_ifunctions(0)
    , m_ShaderId(0)
    , m_programId(0)
{}

Shader::Shader(IFunctions * ifunctions,
               const char * value,
               Shader::Type type,
               const Attributes & attributes)
    : m_value(value)
    , m_type(type)
    , m_ifunctions(ifunctions)
    , m_ShaderId(0)
    , m_programId(0)
    , m_attributes(attributes)
{
    init();
}

Shader::~Shader()
{
    clear();
}

void Shader::setValue(const char *value)
{
    m_value = value;
}

void Shader::setType(Shader::Type type)
{
    m_type = type;
}

void Shader::setIFunctions(IFunctions *ifunctions)
{
    m_ifunctions = ifunctions;
}

void Shader::setAttributes(const Shader::Attributes &attributes)
{
    m_attributes = attributes;
}

const Shader::Attributes &Shader::getAttributes() const
{
    return m_attributes;
}

unsigned int Shader::getProgramId()
{
    return m_programId;
}

unsigned int Shader::getShaderId()
{
    return m_ShaderId;
}

void Shader::init()
{
    switch(m_type)
    {
    case VERTEX:
        m_ShaderId = m_ifunctions->glCreateShader(GL_VERTEX_SHADER);
        break;
    case FRAGMENT:
        m_ShaderId = m_ifunctions->glCreateShader(GL_FRAGMENT_SHADER);
        break;
    default:
        return;
    }
    m_ifunctions->glShaderSource(m_ShaderId, 1, &m_value, NULL); // передать текст шейдера
    m_ifunctions->glCompileShader(m_ShaderId); // скомпилировать шейдер
    m_programId = m_ifunctions->glCreateProgram();
    m_ifunctions->glAttachShader(m_programId, m_ShaderId);
    m_ifunctions->glLinkProgram(m_programId);

    int ok;
    m_ifunctions->glGetProgramiv(m_programId, GL_LINK_STATUS, &ok); // проверить сборку
    if(!ok)
    {
        clear();
        qDebug() << "error attach Shader";
        return;
    }
    for(Attributes::iterator it = m_attributes.begin();
        it != m_attributes.end(); ++it)
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

void Shader::clear()
{
    m_ifunctions->glDeleteShader(m_ShaderId);
    m_ifunctions->glDeleteProgram(m_programId);
}
