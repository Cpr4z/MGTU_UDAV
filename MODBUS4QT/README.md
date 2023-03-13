[[__TOC__]]

# modbus4qt

modbus4qt is a library containing set of classes to work with devices via MODBUS using Qt.

The library works with Qt5. Early versions worked with Qt4, but this option is not tested for now as Qt4 is obsolete.

We have fully functional classes of MODBUS/RTU and MODBUS/TCP clients which can communicate with a server.

Master-RTU program is also fully functional and can be used for testing and debugging.

Work on MODBUS/TCP is on progress.

## Source code and project homepage

Project homepage is https://mt11.net.ru/elvactech-lab/modbus4qt/

You can clone repository using https://mt11.net.ru/elvactech-lab/modbus4qt.git address.

The repo is private for now. You you need access, please ask via e-mail l.kolesnik@m-i.ru

## How to use

To have modbus4qt in your project you should include modbus4qt_depend.pri file into your pro file.

We recommend you use library as git submodule in your project.

After cloning the library you need add it into your project. An example is below. This is only example. You should update dir's and file's names in it for real using.

my_project.pro
----------------------------------
```
TEMPLATE   = subdirs
SUBDIRS   += \
        modbus4qt \
        application

application.depends = \
    modbus4qt

```
----------------------------------

application/application.pro
----------------------------------
```
# Add something like this into your project file:
#
include (path/to/modbus4qt/modbus4qt_depend.pri)
#
# this will add modbus4qt root to include path and will link the library to your app.
```
----------------------------------

After that you can use classes in your application.
