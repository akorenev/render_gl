#ifndef Shader_H
#define Shader_H

#include "ifunctions.h"
#include "shaderinfo.h"


class Shader
{
public:
    typedef std::shared_ptr<Shader> Ptr;
public:
    Shader();
    Shader(IFunctions * ifunctions, const ShaderInfoL & shaderInfoL);
    ~Shader();
    void setShaderInfo(const ShaderInfoL & shaderInfoL);
    const ShaderInfoL & getShaderInfo() const;
    unsigned int getProgramId();
    void init();
private:
    ShaderInfoL m_shaderInfoL;
    IFunctions * m_ifunctions;
    unsigned int m_programId;
};



#endif // Shader_H
