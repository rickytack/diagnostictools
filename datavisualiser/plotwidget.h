/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>

namespace DiagnosticTools {
namespace Internal {

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(QWidget *parent = 0);

    void addValue(quint32 value);
    void reset();

signals:

public slots:

private: // methods
    void drawBackgroundImage();

    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private: // fields
    const quint32 numOfSteps = 70;

    QList <quint32> valuesList;

    quint32 maxValue = 0; // for  normalization

    QImage backgroundImage;
};

} // namespace Internal
} // namespace DiagnosticTools

#endif // PLOTWIDGET_H
