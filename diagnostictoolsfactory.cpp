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

#include "diagnostictoolsfactory.h"
#include "datavisualiser/datavisualiser.h"

#include <QLabel>
#include <QDebug>

namespace DiagnosticTools {
namespace Internal {

DiagnosticToolsFactory::DiagnosticToolsFactory(DataQueryEngine *dataQueryEngine)
{
    qDebug() << "in constructor DiagnosticToolsFactory";
    m_dataQueryEngine = dataQueryEngine;
}
//================================================================================
DiagnosticToolsFactory::~DiagnosticToolsFactory(){

    qDebug() << "in destructor DiagnosticToolsFactory";
}
//================================================================================
Core::NavigationView DiagnosticToolsFactory::createWidget(){

     return Core::NavigationView(new DataVisualiser(m_dataQueryEngine));
}
//================================================================================

} // namespace Internal
} // namespace DiagnosticTools
