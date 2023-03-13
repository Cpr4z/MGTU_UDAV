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

#include <QtGui>
#include <QAction>
#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableView>
#include <QTableWidget>
#include <QVBoxLayout>

#include "main.h"
#include "modbus4qt/server_internal_data.h"
#include "add_cells_dialog.h"
#include "coils_table_model.h"
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create internal server data
    //
    serverInternalData_ = new modbus4qt::ServerInternalData();

    // Create table models for server data
    //
    coilsTableModel_ = new CoilsTableModel(serverInternalData_->coils());

    // Create layout for coils & registers
    //
    QGridLayout* registersLayout = new QGridLayout();
    registersLayout->addWidget(createCoilsGroupBox_(), 0, 0);
    registersLayout->addWidget(createHoldingRegistersGroupBox_(), 0, 1);
    registersLayout->addWidget(createDiscreteInputsGroupBox_(), 1, 0);
    registersLayout->addWidget(createInputRegistersGroupBox_(), 1, 1);

    // Create main layout
    //
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(createServerGroupBox_());
    mainLayout->addLayout(registersLayout);

    // Create central widget
    //
    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Create actions & menus
    //
    createActions_();
    createMenus_();

    // Create status bar
    //
    statusBar();

    // Tweak app window
    //
    setWindowTitle(ApplicationName);
}

//-----------------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

//-----------------------------------------------------------------------------

void
MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(ApplicationName);
    msgBox.setInformativeText(QString("Copyright \nhttp://www.") + OrganizationDomain);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

//-----------------------------------------------------------------------------

void
MainWindow::addCoils()
{
    AddCellsDialog addCellsDialog;
    if (addCellsDialog.exec() == QDialog::Accepted)
    {
        serverInternalData_->addCoils(addCellsDialog.startAddress(), addCellsDialog.cellsQuantity());
    }
}

//-----------------------------------------------------------------------------

void
MainWindow::createActions_()
{
    aboutAction_ = new QAction(tr("&About..."), this);
    aboutAction_->setStatusTip(tr("Show this program's About box"));
    connect(aboutAction_, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction_ = new QAction(tr("About &Qt..."), this);
    aboutQtAction_->setShortcut(tr("Ctrl+Shift+F1"));
    aboutQtAction_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    exitAction_ = new QAction(tr("E&xit"), this);
    exitAction_->setShortcut(tr("Ctrl+Q"));
    exitAction_->setStatusTip(tr("Exit the application"));
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));
}

//-----------------------------------------------------------------------------

void
MainWindow::createMenus_()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(exitAction_);

    helpMenu_ = menuBar()->addMenu(tr("Help"));
    helpMenu_->addAction(aboutAction_);
    helpMenu_->addAction(aboutQtAction_);
}

//-----------------------------------------------------------------------------

QGroupBox*
MainWindow::createCoilsGroupBox_()
{
    QGroupBox* coilsGroupBox = new QGroupBox(tr("Coils"));
    QVBoxLayout* layout = new QVBoxLayout();

    QTableView* coilsTableView_ = new QTableView();
    coilsTableView_->setModel(coilsTableModel_);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton();
    addButton->setText(tr("Add..."));
    connect(addButton, SIGNAL(clicked()), this, SLOT(addCoils()));

    buttonLayout->addWidget(addButton);
    buttonLayout->addStretch();

    layout->addWidget(coilsTableView_);
    layout->addLayout(buttonLayout);

    coilsGroupBox->setLayout(layout);

    return coilsGroupBox;
}

//-----------------------------------------------------------------------------

QGroupBox*
MainWindow::createDiscreteInputsGroupBox_()
{
    QGroupBox* discreteInputsGroupBox = new QGroupBox(tr("Discrete inputs"));
    QVBoxLayout* layout = new QVBoxLayout();

    discreteInputsTable_ = new QTableWidget(0, 2);
    discreteInputsTable_->setHorizontalHeaderLabels(QStringList()
                                           << tr("Address")
                                           << tr("Value"));

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton();
    addButton->setText(tr("Add..."));
    buttonLayout->addWidget(addButton);
    buttonLayout->addStretch();

    layout->addWidget(discreteInputsTable_);
    layout->addLayout(buttonLayout);

    discreteInputsGroupBox->setLayout(layout);

    return discreteInputsGroupBox;
}

//-----------------------------------------------------------------------------

QGroupBox*
MainWindow::createHoldingRegistersGroupBox_()
{
    QGroupBox* holdingRegistersGroupBox = new QGroupBox(tr("Holding Registers"));
    QVBoxLayout* layout = new QVBoxLayout();

    holdingRegistersTable_ = new QTableWidget(0, 2);
    holdingRegistersTable_->setHorizontalHeaderLabels(QStringList()
                                           << tr("Address")
                                           << tr("Value"));

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton();
    addButton->setText(tr("Add..."));
    buttonLayout->addWidget(addButton);
    buttonLayout->addStretch();

    layout->addWidget(holdingRegistersTable_);
    layout->addLayout(buttonLayout);

    holdingRegistersGroupBox->setLayout(layout);

    return holdingRegistersGroupBox;
}

//-----------------------------------------------------------------------------

QGroupBox*
MainWindow::createInputRegistersGroupBox_()
{
    QGroupBox* inputRegistersGroupBox = new QGroupBox(tr("Input Registers"));
    QVBoxLayout* layout = new QVBoxLayout();

    inputRegistersTable_ = new QTableWidget(0, 2);
    inputRegistersTable_->setHorizontalHeaderLabels(QStringList()
                                           << tr("Address")
                                           << tr("Value"));

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addButton = new QPushButton();
    addButton->setText(tr("Add..."));
    buttonLayout->addWidget(addButton);
    buttonLayout->addStretch();

    layout->addWidget(inputRegistersTable_);
    layout->addLayout(buttonLayout);

    inputRegistersGroupBox->setLayout(layout);

    return inputRegistersGroupBox;
}
//-----------------------------------------------------------------------------

QGroupBox*
MainWindow::createServerGroupBox_()
{
    QGroupBox* serverGroupBox = new QGroupBox(tr("MODBUS TCP Server"));
    QHBoxLayout *layout = new QHBoxLayout;

    QLabel* serverAddressLabel = new QLabel(tr("Server address:"));
    serverAddress_ = new QLineEdit("127.0.0.1");

    layout->addWidget(serverAddressLabel);
    layout->addWidget(serverAddress_);

    QLabel* serverPortLabel = new QLabel(tr("Server port:"));
    serverPort_ = new QSpinBox();
    serverPort_->setRange(1, 65535);
    serverPort_->setValue(502);

    listenButton_ = new QPushButton();
    listenButton_->setText(tr("Listen"));
    listenButton_->setCheckable(true);

    layout->addWidget(serverPortLabel);
    layout->addWidget(serverPort_);
    layout->addWidget(listenButton_);

    layout->addStretch();

    serverGroupBox->setLayout(layout);

    return serverGroupBox;
}

//-----------------------------------------------------------------------------
// EOF
