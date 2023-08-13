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
