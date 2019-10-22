#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMMONGUI_LIB)
#  define COMMONGUI_EXPORT Q_DECL_EXPORT
# else
#  define COMMONGUI_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMMONGUI_EXPORT
#endif
