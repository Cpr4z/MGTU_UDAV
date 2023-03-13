#ifndef DRIVERS_GLOBAL_H
#define DRIVERS_GLOBAL_H

#include <QtGlobal>

#if defined(UDAV_DRIVERS_DLL)
    #define UDAV_DRIVERS_EXPORT Q_DECL_EXPORT
#else
    #define UDAV_DRIVERS_EXPORT Q_DECL_IMPORT
#endif // UDAV_DRIVERS_DLL

#endif // DRIVERS_GLOBAL_H