/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#ifndef DIAGNOSTICTOOLSFACTORY_H
#define DIAGNOSTICTOOLSFACTORY_H

#include <coreplugin/inavigationwidgetfactory.h>

#include <QPointer>

namespace DiagnosticTools {
namespace Internal {

class DataQueryEngine;

class DiagnosticToolsFactory : public Core::INavigationWidgetFactory
{
    Q_OBJECT
public:
    explicit DiagnosticToolsFactory(DataQueryEngine *dataQueryEngine);
    ~DiagnosticToolsFactory();

signals:

public slots:

private: // methods
    Core::NavigationView createWidget();

private: // fields
    DataQueryEngine *m_dataQueryEngine;

};

} // namespace Internal
} // namespace DiagnosticTools

#endif // DIAGNOSTICTOOLSFACTORY_H
