/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#include "diagnostictoolsplugin.h"
#include "diagnostictoolsconstants.h"
#include "diagnostictoolsfactory.h"
#include "dataqueryengine/dataqueryengine.h"
// TODO: remove unnecessary
#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/ioutputpane.h>
#include <projectexplorer/buildtargetinfo.h>
#include <projectexplorer/target.h>
#include <cpptools/projectinfo.h>
#include <projectexplorer/buildconfiguration.h>
#include <projectexplorer/buildtargetinfo.h>
#include <projectexplorer/runconfiguration.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
//
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
//
namespace DiagnosticTools {
namespace Internal {

DiagnosticToolsPlugin::DiagnosticToolsPlugin()
{
    //qDebug() << "in constructor DiagnosticToolsPlugin";
    // Create your members
    m_dataQueryEngine = new DataQueryEngine(this);
}
//================================================================================
DiagnosticToolsPlugin::~DiagnosticToolsPlugin(){

    //qDebug() << "DiagnosticToolsPlugin::~DiagnosticToolsPlugin";
    // Unregister objects from the plugin manager's object pool
    // Delete members
}
//================================================================================
bool DiagnosticToolsPlugin::initialize(const QStringList &arguments, QString *errorString){

    //qDebug() << "DiagnosticToolsPlugin::initialize";
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    // add DiagnosticToolsFactory
    m_diagnosticToolsFactory = new DiagnosticToolsFactory(m_dataQueryEngine);
    m_diagnosticToolsFactory->setDisplayName("DiagnosticTools");
    m_diagnosticToolsFactory->setPriority(1);
    addAutoReleasedObject(m_diagnosticToolsFactory);
    //
    setConnections();
    //
    return true;
}
//================================================================================
void DiagnosticToolsPlugin::extensionsInitialized(){

    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}
//================================================================================
ExtensionSystem::IPlugin::ShutdownFlag DiagnosticToolsPlugin::aboutToShutdown(){

    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}
//================================================================================
void DiagnosticToolsPlugin::onRunControlStarted(ProjectExplorer::RunControl *rc){

    m_runControlPtr = rc;

    qDebug() << "DiagnosticToolsPlugin::onRunControlStarted";
    qDebug() << "PID:" << rc->applicationProcessHandle().pid();
    qDebug() <<"";
    connect(m_runControlPtr,
            &ProjectExplorer::RunControl::applicationProcessHandleChanged,
            this, &DiagnosticToolsPlugin::onApplicationHandleChanged);
}
//================================================================================
void DiagnosticToolsPlugin::onRunControlFinished(ProjectExplorer::RunControl *rc){

    Q_UNUSED(rc)
    qDebug() << "DiagnosticToolsPlugin::onRunControlFinished";
    qDebug() <<"";

    m_runControlPtr = nullptr;
    m_dataQueryEngine->stopDataQuery();
}
//================================================================================
void DiagnosticToolsPlugin::onApplicationHandleChanged(){

    qDebug() << "DiagnosticToolsPlugin::onApplicationHandleChanged";

    if (m_runControlPtr && m_runControlPtr->applicationProcessHandle().isValid()){
        m_dataQueryEngine->setPid(m_runControlPtr->applicationProcessHandle().pid());
        m_dataQueryEngine->startDataQuery();
    }
    else{
        qDebug() << "Process handle is invalid";
    }
    qDebug() << "";
}
//================================================================================
void DiagnosticToolsPlugin::setConnections(){

    // plugin
    connect(ProjectExplorer::ProjectExplorerPlugin::instance(),
      &ProjectExplorer::ProjectExplorerPlugin::runControlStarted,
            this, &DiagnosticToolsPlugin::onRunControlStarted);

    connect(ProjectExplorer::ProjectExplorerPlugin::instance(),
      &ProjectExplorer::ProjectExplorerPlugin::runControlFinished,
            this, &DiagnosticToolsPlugin::onRunControlFinished);

}

} // namespace Internal
} // namespace DiagnosticTools
