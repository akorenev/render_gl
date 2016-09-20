#ifndef Shader_H
#define Shader_H
#include "ifunctions.h"
#include <memory>


class Shader
{
public:
    enum Type
    {
        VERTEX,
        FRAGMENT,
        NONE
    };
    typedef std::map<const char *, int> Attributes;
    typedef std::shared_ptr<Shader> Ptr;
public:
    Shader();
    Shader(IFunctions * ifunctions,
           const char * value,
           Type type,
           const Attributes & attributes = Attributes());
    ~Shader();
    void setValue(const char * value);
    void setType(Type type);
    void setIFunctions(IFunctions * ifunctions);
    void setAttributes(const Attributes & attributes);
    const Attributes & getAttributes() const;
    unsigned int getProgramId();
    unsigned int getShaderId();
    void init();
private:
    void clear();
    const char * m_value;
    Type m_type;
    IFunctions * m_ifunctions;
    unsigned int m_ShaderId;
    unsigned int m_programId;
    Attributes m_attributes;
};

#endif // Shader_H
