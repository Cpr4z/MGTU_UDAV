/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://mt11.net.ru
 *****************************************************************************/

/*****************************************************************************
* This file is part of modbus4qt.
*
* modbus4qt is free software: you can redistribute it and/or modify it under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any
* later version.
*
* modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* modbus4qt.
*
* If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef MODBUS4QT_GLOBAL_H
#define MODBUS4QT_GLOBAL_H

#include <QtCore/qglobal.h>

// MODBUS4QT_VERSION is (major << 16) + (minor << 8) + patch.

#define MODBUS4QT_VERSION       0x010000
#define MODBUS4QT_VERSION_STR   "1.0.0"

#if defined(Q_WS_WIN) || defined(Q_WS_S60)

    #if defined(_MSC_VER) /* MSVC Compiler */
    /* template-class specialization 'identifier' is already instantiated */
    #pragma warning(disable: 4660)
    #endif // _MSC_VER


    #ifdef MODBUS4QT_DLL

        #if defined(MODBUS4QT_MAKEDLL)	// create a modbus4qt dll library
            #define MODBUS4QT_EXPORT __declspec(dllexport)
            #define MODBUS4QT_TEMPLATEDLL
        #else							// use a modbus4qt dll library
            #define MODBUS4QT_EXPORT __declspec(dllimport)
        #endif

    #endif // MODBUS4QT_DLL

#else // Q_WS_WIN || Q_WS_S60

    #if defined(MODBUS4QT_MAKEDLL)
        #define MODBUS4QT_EXPORT Q_DECL_EXPORT
    #else
        #define MODBUS4QT_EXPORT Q_DECL_IMPORT
    #endif // MODBUS4QT_MAKEDLL
#endif

#endif // MODBUS4QT_GLOBAL_H
