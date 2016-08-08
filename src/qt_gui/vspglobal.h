#ifndef VSPGLOBAL_H
#define VSPGLOBAL_H

#include <QtGlobal>

#ifdef DLL
# ifndef VSP_EXPORT
#  ifdef Q_OS_WIN
#   define VSP_EXPORT __declspec(dllexport)
#  endif
#  ifndef VSP_EXPORT
#   define VSP_EXPORT
#  endif
# endif
#endif

#ifndef VSP_EXPORT
#define VSP_EXPORT
#endif

#endif // VSPGLOBAL_H
