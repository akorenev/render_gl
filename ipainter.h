#ifndef IPAINTER_H
#define IPAINTER_H

#include "ifunctions.h"
#include "model.h"
#include <stack>
#include <QMatrix4x4>


struct IPainterInfo
{
    typedef std::shared_ptr<IPainterInfo> Ptr;
    typedef std::stack<QMatrix4x4> MatrixStack;
    void setStackMatrix(const MatrixStack & matrixStack);
    bool emptyMatrix();
    void popMatrix();
    void pushMatrix(const QMatrix4x4 & matrix);
    const QMatrix4x4 & topMatrix();
private:
    std::stack<QMatrix4x4> m_matrixStack;
};

class ipainter
{
public:
    ipainter();
    virtual ~ipainter();
    virtual void draw(IPainterInfo::Ptr painterInfo) = 0;
    virtual void setModel(Model * model);
    virtual Model * getModel();
    virtual void setIFunctions(IFunctions * iFunctions);
protected:
    IFunctions * m_IFunctions;
    Model * m_model;
};

typedef std::list<ipainter*> IPainterL;

#endif // IPAINTER_H
