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

#include "dataqueryengine.h"

#include <QDebug>
#include <QTimer>

#define TIMER_TIMEOUT 200

namespace DiagnosticTools {
namespace Internal {

//
DataQueryEngine::DataQueryEngine(QObject *parent) : QObject(parent),
    m_queryTimer(new QTimer(this))
{
    connect(m_queryTimer, &QTimer::timeout, this, &DataQueryEngine::getData);
}
//================================================================================
DataQueryEngine::~DataQueryEngine(){

    stopDataQuery();
}
//================================================================================
void DataQueryEngine::setPid(quint64 pid){

    m_currentPid = pid;
}
//================================================================================
void DataQueryEngine::startDataQuery(){

    bool isStartSuccessful = false;

#ifdef Q_OS_WIN

    m_hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, (DWORD)(m_currentPid));

    if (m_hProcess != NULL){
        // get init data for cpu query
        SYSTEM_INFO sysInfo;
        FILETIME ftime, fsys, fuser;

        GetSystemInfo(&sysInfo);
        numProcessors = sysInfo.dwNumberOfProcessors;

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&lastCPU, &ftime, sizeof(FILETIME));

        GetProcessTimes(m_hProcess, &ftime, &ftime, &fsys, &fuser);
        memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
        memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

        isStartSuccessful = true;
    }

#elif defined Q_OS_LINUX

    // file /proc/pid/stat
    std::string pidString = std::to_string(m_currentPid);
    stat_filepath = std::string("/proc/") + pidString + std::string("/stat");
    bzero(&last_usage, sizeof(struct pstat));

    // check if files exist
    if(std::ifstream(stat_filepath) && std::ifstream("/proc/stat"))
        isStartSuccessful = true;
#elif defined Q_OS_MAC
    kern_return_t kret;

    if((kret=task_for_pid(current_task(), m_currentPid, &task)) == KERN_SUCCESS) {
        lastTime = mach_absolute_time();
        lastCPU = 0;

        task_absolutetime_info timeInfo;
        mach_msg_type_number_t info_count = TASK_ABSOLUTETIME_INFO_COUNT;

        if (KERN_SUCCESS == task_info(task,
                                      TASK_ABSOLUTETIME_INFO,
                                      (task_info_t)&timeInfo,
                                      &info_count))
        {
            lastCPU = timeInfo.total_user+timeInfo.total_system;
        }

        isStartSuccessful = true;
    }
    else
    {
        qDebug() << "Attach to pid" << m_currentPid << "failed:" << mach_error_string(kret);
    }

#else
#error "current OS is not supported"
#endif

    // start query
    if(isStartSuccessful){
        m_queryTimer->start(TIMER_TIMEOUT);
        emit started();
    }
    else qWarning() << "RamDataQuery::startDataQuery: start fail";
}
//================================================================================
void DataQueryEngine::stopDataQuery(){

    m_queryTimer->stop();
#ifdef Q_OS_WIN
    if(m_hProcess != NULL){
        CloseHandle(m_hProcess);
        m_hProcess = NULL;
        numProcessors = 0;
    }
#elif defined Q_OS_LINUX
#elif defined Q_OS_MAC
#else
#error "current OS is not supported"
#endif
    emit stopped();
}
//================================================================================
#ifdef Q_OS_WIN
void DataQueryEngine::getData(){

    if (m_hProcess == NULL) return;

    // cpu query
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(m_hProcess, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
            (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    //qDebug() << "percent CPU: " << percent * 100;
    emit cpuDataUpdated(percent * 100);

    // ram query
    PROCESS_MEMORY_COUNTERS pmc;
    if ( GetProcessMemoryInfo(  m_hProcess, &pmc, sizeof(pmc)) ){

//        qDebug() << "pmc.WorkingSetSize" << pmc.WorkingSetSize/1024;
//        qDebug() << "pmc.PagefileUsage" << pmc.PagefileUsage/1024;
//        qDebug() << "pmc.PeakPagefileUsage" << pmc.PeakPagefileUsage/1024;

        emit ramDataUpdated(pmc.WorkingSetSize/1024,
                            pmc.PagefileUsage/1024, pmc.PeakPagefileUsage/1024);
    }
}
#elif defined Q_OS_LINUX
void DataQueryEngine::getData(){

    // get usage
    pstat cur_pstat;
    //read values from /proc/pid/stat
    bzero(&cur_pstat, sizeof(struct pstat));
    std::string p_pid, comm, state, ppid, pgrp, session, tty_nr;
    long tpgid, flags, minflt, cminflt, majflt, cmajflt;
    long priority, nice, o, itrealvalue, starttime;
    unsigned long vsize;
    long rss;
    //read values from /proc/pid/stat
    std::ifstream stat_stream(stat_filepath,std::ios_base::in);
    if(stat_stream.fail()) return;
    stat_stream >> p_pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> cur_pstat.utime_ticks >> cur_pstat.stime_ticks
                >> cur_pstat.cutime_ticks >> cur_pstat.cstime_ticks >> priority >> nice
                >> o >> itrealvalue >> starttime >> vsize >> rss;

    //read+calc cpu total time from /proc/stat
    const int numOfCpuValues = 10;
    long unsigned int cpu_time[numOfCpuValues];
    bzero(cpu_time, sizeof(cpu_time));
    std::string cpu;
    // file /proc/stat
    std::ifstream stat_stream_2("/proc/stat", std::ios_base::in);
    if(stat_stream_2.fail()) return;
    stat_stream_2 >> cpu >> cpu_time[0] >> cpu_time[1] >> cpu_time[2] >> cpu_time[3]
            >> cpu_time[4] >> cpu_time[5] >> cpu_time[6] >> cpu_time[7]
            >> cpu_time[8] >> cpu_time[9];

    for(int i = 0; i < numOfCpuValues; ++i) cur_pstat.cpu_total_time += cpu_time[i];

    // calculate cpu usage
    const long unsigned int total_time_diff = cur_pstat.cpu_total_time -
                                              last_usage.cpu_total_time;

    double ucpu_usage = 100 * (((cur_pstat.utime_ticks + cur_pstat.cutime_ticks)
                    - (last_usage.utime_ticks + last_usage.cutime_ticks))
                    / (double) total_time_diff);

    double scpu_usage = 100 * ((((cur_pstat.stime_ticks + cur_pstat.cstime_ticks)
                    - (last_usage.stime_ticks + last_usage.cstime_ticks))) /
                    (double) total_time_diff);
    int total = ucpu_usage + scpu_usage;

    // refresh last usage value
    last_usage = cur_pstat;

    //qDebug() << "total:" << total;
    emit cpuDataUpdated(total);

    //calculate ram
    long pageSizeKb = sysconf(_SC_PAGE_SIZE) / 1024;
    double vmUsage     = vsize / 1024.0;
    double residentSet = rss * pageSizeKb;
    emit ramDataUpdated(residentSet, vmUsage, 0);
    //qDebug() << "residentSet" << residentSet;
    //qDebug() << "vmUsage" << vmUsage;
}
#elif defined Q_OS_MAC


void DataQueryEngine::getData(){
    task_vm_info vm_info;
    mach_msg_type_number_t info_count = TASK_VM_INFO_COUNT;

    if (KERN_SUCCESS == task_info(task,
                                  TASK_VM_INFO,
                                  (task_info_t)&vm_info,
                                  &info_count))
    {
        emit ramDataUpdated(
                    vm_info.resident_size / 1024.0,
                    vm_info.internal / 1024.0,
                    vm_info.resident_size_peak / 1024.0 );
    }

    task_absolutetime_info timeInfo;
    info_count = TASK_ABSOLUTETIME_INFO_COUNT;
    if (KERN_SUCCESS == task_info(task,
                                  TASK_ABSOLUTETIME_INFO,
                                  (task_info_t)&timeInfo,
                                  &info_count
                                  ))
    {
        quint64 now = mach_absolute_time();
        quint64 cpuTime = timeInfo.total_user+timeInfo.total_system;

        double cpu = (double)(cpuTime - lastCPU) / (now-lastTime) * 100.0; // in percents

        lastCPU = cpuTime;
        lastTime = now;

        emit cpuDataUpdated(cpu);
    }

}
#else
#error "current OS is not supported"
#endif

} // namespace Internal
} // namespace DiagnosticTools
