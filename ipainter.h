#ifndef IPAINTER_H
#define IPAINTER_H

#include "ifunctions.h"
#include "model.h"

class ipainter
{
public:
    ipainter();
    virtual ~ipainter();
    virtual void draw() = 0;
    virtual void setModel(Model * model);
    virtual Model * getModel();
    void setIFunctions(IFunctions * iFunctions);
protected:
    IFunctions * m_IFunctions;
    Model * m_model;
};

typedef std::list<ipainter*> IPainterL;

#endif // IPAINTER_H
