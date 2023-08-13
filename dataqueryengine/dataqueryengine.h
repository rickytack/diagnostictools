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

#ifndef DATAQUERYENGINE_H
#define DATAQUERYENGINE_H


#include <QObject>

#ifdef Q_OS_WIN
#include "windows.h"
#include "TCHAR.h"
#include "pdh.h"
#include "psapi.h"
#elif defined Q_OS_LINUX
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#elif defined Q_OS_MAC
#include <mach/mach.h>
#include <mach/mach_time.h>
#else
#error "current OS is not supported"
#endif

class QTimer;

namespace DiagnosticTools {
namespace Internal {

class DataQueryEngine : public QObject
{
    Q_OBJECT
public:
    explicit DataQueryEngine(QObject *parent = 0);
    ~DataQueryEngine();

    void setPid(quint64 pid);

    void startDataQuery();
    void stopDataQuery();

signals:
    void started();
    void stopped();

    void cpuDataUpdated(int percent);
    void ramDataUpdated(quint32 memory, quint32 memory_2, quint32 peakMemory_2);

public slots:

private slots:
    void getData();

private: // fields
    quint64 m_currentPid;

    QTimer *m_queryTimer;

#ifdef Q_OS_WIN
    // cpu
    ULARGE_INTEGER lastCPU;
    ULARGE_INTEGER lastSysCPU;
    ULARGE_INTEGER lastUserCPU;
    int numProcessors = 0;
    HANDLE m_hProcess = NULL;
#elif defined Q_OS_LINUX

    std::string stat_filepath;

    // cpu
    struct pstat {
        long unsigned int utime_ticks;
        long int cutime_ticks;
        long unsigned int stime_ticks;
        long int cstime_ticks;
        long unsigned int vsize;
        long unsigned int cpu_total_time;
    };
    pstat last_usage;
#elif defined Q_OS_MAC
    task_t task;
    quint64 lastCPU;
    quint64 lastTime;
#else
#error "current OS is not supported"
#endif

};

} // namespace Internal
} // namespace DiagnosticTools


#endif // DATAQUERYENGINE_H
