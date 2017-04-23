/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#ifndef DATAVISUALISER_H
#define DATAVISUALISER_H

#include <QWidget>
//#include <QTimer> // temp

class QTimer;

namespace Ui {
class DataVisualiser;
}

namespace DiagnosticTools {
namespace Internal {

class DataQueryEngine;

class DataVisualiser : public QWidget
{
    Q_OBJECT

public:
    explicit DataVisualiser(DataQueryEngine *dataQueryEngine, QWidget *parent = 0);
    ~DataVisualiser();

public slots:
    void startSession();
    void stopSession();

    void updateRamData(quint32 workingSetSize, quint32 pagefileUsage,
                       quint32 peakPagefileUsage);
    void updateCpuData(int percent);

private slots:
    //void testSlot();
    void onSessionTimerTimeout();

private: // methods
    void setConnection();

private: // feilds
    Ui::DataVisualiser *ui;

    DataQueryEngine *m_dataQueryEngine;

    QTimer *sessionTimer;
    qint64 secondsCount = 0;
};

} // namespace Internal
} // namespace DiagnosticTools

#endif // DATAVISUALISER_H
