#include "ipainter.h"

ipainter::ipainter()
    : m_IFunctions(0)
    , m_model(0){}

ipainter::~ipainter()
{
    if(m_model)
        delete m_model;
}

void ipainter::setModel(Model *model)
{
    m_model = model;
}

Model * ipainter::getModel()
{
    return m_model;
}

void ipainter::setIFunctions(IFunctions * iFunctions)
{
    m_IFunctions = iFunctions;
}

void IPainterInfo::setStackMatrix(const IPainterInfo::MatrixStack & matrixStack)
{
    m_matrixStack = matrixStack;
}

bool IPainterInfo::emptyMatrix()
{
    return m_matrixStack.empty();
}

void IPainterInfo::popMatrix()
{
    m_matrixStack.pop();
}

void IPainterInfo::pushMatrix(const QMatrix4x4 &matrix)
{
    m_matrixStack.push(matrix);
}

const QMatrix4x4 &IPainterInfo::topMatrix()
{
    return m_matrixStack.top();
}
