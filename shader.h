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
    Shader(IFunctions * ifunctions, const ShaderInfoV & shaderInfoV);
    ~Shader();
    void setShaderInfo(const ShaderInfoV & shaderInfoV);
    const ShaderInfoV & getShaderInfo() const;
    unsigned int getProgramId();
    bool init();
    bool isInit();
private:
    ShaderInfoV m_shaderInfoV;
    IFunctions * m_ifunctions;
    unsigned int m_programId;
    bool m_isInit;
};



#endif // Shader_H
