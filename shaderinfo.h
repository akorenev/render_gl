#ifndef SHADERINFO_H
#define SHADERINFO_H
#include <vector>
#include <memory>
#include <algorithm>
#include <string>

class Attribute
{
public:
    Attribute()
        : m_key(-1)
        , m_value(""){}
    Attribute(const char * value, int key = 0)
        : m_key(key)
        , m_value(value){}
    int getKey() const { return m_key;}
    const char * getValue() const { return m_value;}
    void setKey(int key) { m_key = key;}
    void setValue(const char * value){ m_value = value;}
private:
    int m_key;
    const char * m_value;
};

class KeyMatchAttribute
{
public:
    KeyMatchAttribute(const char * name)
        : m_name(name){}
    bool operator ()(const Attribute & attribute)
    {
        return strcmp(attribute.getValue(), m_name) == 0 ? true : false;
    }
private:
    const char * m_name;
};

struct ShaderInfo
{
    enum Type
    {
        VERTEX,
        FRAGMENT,
        NONE
    };
    enum TypeAttributes
    {
        UNIFORM,
        LOCATION
    };
    typedef std::vector<Attribute> Attributes;
    typedef std::shared_ptr<ShaderInfo> Ptr;
    ShaderInfo(const char * _value,
               Type _type,
               const Attributes & _locationAttributes = Attributes(),
               const Attributes & _uniformLocationAttributes = Attributes())
        : value(_value)
        , locationAttributes(_locationAttributes)
        , uniformLocationAttributes(_uniformLocationAttributes)
        , type(_type)
        , shaderId(0){}
    int getKeyAttribute(const char * nameAttribute, TypeAttributes typeAttributes) const
    {
        const Attributes & attributes = getAttributesByType(typeAttributes);
        ShaderInfo::Attributes::const_iterator it =
                std::find_if(attributes.begin(), attributes.end(),
                     KeyMatchAttribute(nameAttribute));
        return it != attributes.cend() ? (*it).getKey() : -1;
    }
    const char * value;
    Attributes locationAttributes;
    Attributes uniformLocationAttributes;
    Type type;
    unsigned int shaderId;
private:
    const Attributes & getAttributesByType(TypeAttributes typeAttributes) const
    {
        switch (typeAttributes)
        {
        case UNIFORM:
            return uniformLocationAttributes;
        case LOCATION:
            return locationAttributes;
        }
    }
};
typedef std::vector<ShaderInfo::Ptr> ShaderInfoV;

#endif // SHADERINFO_H
