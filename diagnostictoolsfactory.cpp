/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#include "diagnostictoolsfactory.h"
#include "datavisualiser/datavisualiser.h"

#include <QLabel>
#include <QDebug>

namespace DiagnosticTools {
namespace Internal {

DiagnosticToolsFactory::DiagnosticToolsFactory(DataQueryEngine *dataQueryEngine)
{
    qDebug() << "in constructor DiagnosticToolsFactory";
    m_dataQueryEngine = dataQueryEngine;
}
//================================================================================
DiagnosticToolsFactory::~DiagnosticToolsFactory(){

    qDebug() << "in destructor DiagnosticToolsFactory";
}
//================================================================================
Core::NavigationView DiagnosticToolsFactory::createWidget(){

     return Core::NavigationView(new DataVisualiser(m_dataQueryEngine));
}
//================================================================================

} // namespace Internal
} // namespace DiagnosticTools
