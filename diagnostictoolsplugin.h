/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#pragma once

#include "diagnostictools_global.h"

#include <extensionsystem/iplugin.h>
#include <projectexplorer/runconfiguration.h>

namespace DiagnosticTools {
namespace Internal {

class DataQueryEngine;
class DiagnosticToolsOutputPane;
class DiagnosticToolsFactory;

class DiagnosticToolsPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "diagnostictools.json")

public:
    DiagnosticToolsPlugin();
    ~DiagnosticToolsPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private slots:
    void onRunControlStarted(ProjectExplorer::RunControl *rc);
    void onRunControlFinished(ProjectExplorer::RunControl *rc);
    void onApplicationHandleChanged();

private: // methods
    void setConnections();
    //void triggerAction();

private: // feilds
    DataQueryEngine *m_dataQueryEngine;

    //DiagnosticToolsOutputPane *m_outputPane;
    DiagnosticToolsFactory *m_diagnosticToolsFactory;

    ProjectExplorer::RunControl* m_runControlPtr = nullptr;
};

} // namespace Internal
} // namespace DiagnosticTools

