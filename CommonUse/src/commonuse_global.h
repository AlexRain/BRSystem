#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMMONUSE_LIB)
#  define COMMONUSE_EXPORT Q_DECL_EXPORT
# else
#  define COMMONUSE_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMMONUSE_EXPORT
#endif
