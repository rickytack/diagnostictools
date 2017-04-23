/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#include "plotwidget.h"

#include <QPainter>
#include <QDebug>

namespace DiagnosticTools {
namespace Internal {

PlotWidget::PlotWidget(QWidget *parent) : QWidget(parent)
{

}
//================================================================================
void PlotWidget::addValue(quint32 value){

    if(valuesList.size() == numOfSteps) valuesList.removeFirst();
    valuesList.append(value);

    // refresh maxValue
    maxValue = valuesList.first();
    foreach (quint32 value, valuesList)
        if(value > maxValue) maxValue = value;

    update();
}
//================================================================================
void PlotWidget::reset(){

    maxValue = 0;
    valuesList.clear();
    update();
}
//================================================================================
void PlotWidget::drawBackgroundImage(){

    backgroundImage = QImage(width(), height(), QImage::Format_ARGB4444_Premultiplied);
    QPainter painter(&backgroundImage);

    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawRect(rect());

    painter.setPen(QPen(Qt::gray, 1));

    float numOfLines = 10;
    for(float i = 0; i < numOfLines; ++i){
        painter.drawLine(0, i/numOfLines*height(), width(), i/numOfLines*height());
        painter.drawLine(i/numOfLines*width(), 0, i/numOfLines*width(), height());
    }
}
//================================================================================
void PlotWidget::showEvent(QShowEvent *){

    drawBackgroundImage();
}
//================================================================================
void PlotWidget::resizeEvent(QResizeEvent *){

    drawBackgroundImage();
}
//================================================================================
void PlotWidget::paintEvent(QPaintEvent*){

    QPainter painter(this);

    // paint background image
    if(!backgroundImage.isNull()) painter.drawImage(rect(), backgroundImage);

    // paint plot
    painter.setRenderHint(QPainter::Antialiasing);
    //painter.setPen(QPen(Qt::green, 1));

    if(maxValue == 0) return;

    for(int i = 0; i < valuesList.size() - 1; ++i){

        // calculate points
        int xposShift = ( (float)(numOfSteps - valuesList.size() + 1) / (float)numOfSteps )*width();
        int maxValueHeight = height()*0.9;

        int xpos_1 = (float)i / (float)numOfSteps * width();
        int ypos_1 = (float)valuesList[i] / (float)maxValue * maxValueHeight;
        QPoint point_1(xpos_1 + xposShift, height() - ypos_1);

        int xpos_2 = (float)(i + 1) / (float)numOfSteps * width();
        int ypos_2 = (float)valuesList[i + 1] / (float)maxValue * maxValueHeight;
        QPoint point_2(xpos_2 + xposShift, height() - ypos_2);

        // draw
        painter.setBrush(QBrush(QColor(0, 130, 0)));
        painter.setPen(Qt::NoPen);

        QPolygon pPolygon;
        pPolygon<< point_1 << point_2 <<
                   QPoint(point_2.x(), height()) << QPoint(point_1.x(), height());
        painter.drawPolygon(pPolygon);
    }
}
//================================================================================
} // namespace Internal
} // namespace DiagnosticTools
