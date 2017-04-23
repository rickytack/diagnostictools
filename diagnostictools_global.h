/****************************************************************************
**
** Copyright (C) 2017 Ricky Tack
** Contact: rickytack9@gmail.com
**
****************************************************************************/

#pragma once

#include <QtGlobal>

#if defined(DIAGNOSTICTOOLS_LIBRARY)
#  define DIAGNOSTICTOOLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DIAGNOSTICTOOLSSHARED_EXPORT Q_DECL_IMPORT
#endif
