#include "shader.h"
#include <iostream>

GLuint loadShader(const char * value, GLenum type, IFunctions * ifunctions);

Shader::Shader()
    : m_ifunctions(0)
    , m_programId(0)
    , m_isInit(0)
{}

Shader::Shader(IFunctions * ifunctions,
               const ShaderInfoV & shaderInfoV)
    : m_shaderInfoV(shaderInfoV)
    , m_ifunctions(ifunctions)
    , m_programId(0)
    , m_isInit(0)
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

bool Shader::init()
{
    m_isInit = true;
    for(auto & shaderInfo : m_shaderInfoV)
    {
        switch(shaderInfo->type)
        {
        case ShaderInfo::VERTEX:
            shaderInfo->shaderId = loadShader(shaderInfo->value, GL_VERTEX_SHADER, m_ifunctions);
            break;
        case ShaderInfo::FRAGMENT:
            shaderInfo->shaderId = loadShader(shaderInfo->value, GL_FRAGMENT_SHADER, m_ifunctions);
            break;
        default:
            continue;
        }
        if(shaderInfo->shaderId == 0) // you have some errors
        {
            m_isInit = false;
            return m_isInit;
        }
    }
    // Create the program object
    m_programId = m_ifunctions->glCreateProgram();
    if(m_programId == 0)  // you have some errors
    {
        m_isInit = false;
        return m_isInit;
    }
    for(auto & shaderInfo : m_shaderInfoV)
        m_ifunctions->glAttachShader(m_programId, shaderInfo->shaderId);

    int i = 0;
    for(auto & shaderInfo : m_shaderInfoV) // attributes location
    {
        for(ShaderInfo::Attributes::iterator it = shaderInfo->locationAttributes.begin();
            it != shaderInfo->locationAttributes.end(); ++it)
        {
            m_ifunctions->glBindAttribLocation(m_programId, i, (*it).getValue()); // check exist attribute
            (*it).setKey(i++);
        }
    }
    // Link the program
    m_ifunctions->glLinkProgram(m_programId);
    for(auto & shaderInfo : m_shaderInfoV) // attributes uniform location
    {
        for(ShaderInfo::Attributes::iterator it = shaderInfo->uniformLocationAttributes.begin();
            it != shaderInfo->uniformLocationAttributes.end(); ++it)
        {
            int id = m_ifunctions->glGetUniformLocation(m_programId, (*it).getValue()); // check exist attribute
            if(id == -1)
            {
                std::cout << "could not bind uniform location attribute " << (*it).getValue() << std::endl;
                m_isInit = false;
                return m_isInit;
            }
            else
            {
                (*it).setKey(id);
            }
        }
    }

    GLint linked;
    // Check the link status
    m_ifunctions->glGetProgramiv(m_programId, GL_LINK_STATUS, &linked);
    if(!linked) // you have some errors
    {
        GLint infoLen = 0;
        m_ifunctions->glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1)
        {
            char* infoLog = (char *)malloc(sizeof(char) * infoLen);
            m_ifunctions->glGetProgramInfoLog(m_programId, infoLen, NULL, infoLog);
            free(infoLog);
        }
        m_ifunctions->glDeleteProgram(m_programId);
        m_isInit = false;
        return m_isInit;
    }

    return m_isInit;
}

bool Shader::isInit()
{
    return m_isInit;
}

GLuint loadShader(const char * value, GLenum type, IFunctions * ifunctions)
{
    // Create the shader object
    GLuint shader = ifunctions->glCreateShader(type);
    if(shader == 0)
        return 0;
    // Load the shader source
    ifunctions->glShaderSource(shader, 1, &value, NULL);

    // Compile the shader
    ifunctions->glCompileShader(shader);
    // Check the compile status
    GLint compiled;
    ifunctions->glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled)
    {
        GLint infoLen = 0;
        ifunctions->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1)
        {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            ifunctions->glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cout << " shader error compile " << QString(infoLog).toStdString() << std::endl;
            free(infoLog);
        }
        ifunctions->glDeleteShader(shader);
        return 0;
    }
    return shader;
}
