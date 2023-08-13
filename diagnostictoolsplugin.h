/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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

