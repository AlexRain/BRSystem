#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DATABASE_LIB)
#  define DATABASE_EXPORT Q_DECL_EXPORT
# else
#  define DATABASE_EXPORT Q_DECL_IMPORT
# endif
#else
# define DATABASE_EXPORT
#endif
