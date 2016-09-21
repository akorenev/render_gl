#ifndef SHADERINFO_H
#define SHADERINFO_H
#include <map>
#include <list>
#include <memory>

struct ShaderInfo
{
    enum Type
    {
        VERTEX,
        FRAGMENT,
        NONE
    };
    typedef std::map<const char *, int> Attributes;
    typedef std::shared_ptr<ShaderInfo> Ptr;
    ShaderInfo(const char * _value,
               Type _type,
               const Attributes & _attributes = Attributes())
        : value(_value)
        , attributes(_attributes)
        , type(_type)
        , shaderId(0){}

    const char * value;
    Attributes attributes;
    Type type;
    unsigned int shaderId;
};
typedef std::list<ShaderInfo::Ptr> ShaderInfoL;

#endif // SHADERINFO_H
