/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#include "datavisualiser.h"
#include "../dataqueryengine/dataqueryengine.h"

using namespace DiagnosticTools::Internal;
#include "ui_datavisualiser.h"

#include <QTimer>
#include <QDebug>

#define SESSION_TIMER_TICK 1000

namespace DiagnosticTools {
namespace Internal {

DataVisualiser::DataVisualiser(DataQueryEngine *dataQueryEngine, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataVisualiser),
    sessionTimer(new QTimer(this))
{
    qDebug() << "in constructor DataVisualiser";
    ui->setupUi(this);

    m_dataQueryEngine = dataQueryEngine;

    setConnection();
}
//================================================================================
DataVisualiser::~DataVisualiser()
{
    qDebug() << "in destructor DataVisualiser";
    stopSession();
    delete ui;
}
//================================================================================
void DataVisualiser::startSession(){
    qDebug() << "DataVisualiser::startSession";
    sessionTimer->start(SESSION_TIMER_TICK);
    secondsCount = 0;
}
//================================================================================
void DataVisualiser::stopSession(){

    sessionTimer->stop();
    ui->timeOfSessionLabel->setNum(0);

    ui->memoryPlot->reset();
    ui->dedicatedMemoryPlot->reset();
    ui->cpuLoadPercentPlot->reset();
}
//================================================================================
void DataVisualiser::updateRamData(quint32 workingSetSize, quint32 pagefileUsage,
                                   quint32 peakPagefileUsage){
    // memory
    ui->memoryPlot->addValue(workingSetSize);
    ui->currentMemoryLabel->setText(QString::number((int)workingSetSize) + " Кб");
    // dedicated memory
    ui->dedicatedMemoryPlot->addValue(pagefileUsage);
    ui->currentDedicatedMemoryLabel->setText(QString::number((int)pagefileUsage) + " Кб");
    ui->peakDedicatedMemoryLabel->setText(QString::number((int)peakPagefileUsage) + " Кб");
}
//================================================================================
void DataVisualiser::updateCpuData(int percent){

    //qDebug() << "DataVisualiser::updateCpuData percent" << percent;
    ui->cpuLoadPercentPlot->addValue(percent);
    ui->cpuLoadPercenLabel->setText(QString::number(percent) + " %");
}
//================================================================================
void DataVisualiser::onSessionTimerTimeout(){ // TODO: maybe lambda

    secondsCount ++;
    ui->timeOfSessionLabel->setText(QString::number(secondsCount) + " c");
}
//================================================================================
void DataVisualiser::setConnection(){

    connect(sessionTimer, &QTimer::timeout, this, &DataVisualiser::onSessionTimerTimeout);

    connect(m_dataQueryEngine, &DataQueryEngine::ramDataUpdated,
            this, &DataVisualiser::updateRamData);
    connect(m_dataQueryEngine, &DataQueryEngine::cpuDataUpdated,
            this, &DataVisualiser::updateCpuData);
    connect(m_dataQueryEngine, &DataQueryEngine::started,
            this, &DataVisualiser::startSession);
    connect(m_dataQueryEngine, &DataQueryEngine::stopped,
            this, &DataVisualiser::stopSession);
}

} // namespace Internal
} // namespace DiagnosticTools
