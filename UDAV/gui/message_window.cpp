#include "message_window.h"

#include <QTableWidget>
#include <QPushButton>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCloseEvent>
#include <QHeaderView>
#include <QFont>

#include "core/core.h"

namespace udav_gui
{

//-----------------------------------------------------------------------------

MessageWindow::MessageWindow(QWidget *parent) :
    QWidget(parent),
    autoClose_(false),
    canClose_(false),
    rowCount_(0)
{
    messageBrowser_ = new QTableWidget(this);
    messageBrowser_->setMinimumWidth(600);
    messageBrowser_->setColumnCount(1);
    messageBrowser_->setShowGrid(false);
    messageBrowser_->setSortingEnabled(false);
    messageBrowser_->horizontalHeader()->hide();
    messageBrowser_->verticalHeader()->hide();

    allButton_ = new QPushButton(tr("&All"));
    allButton_->setCheckable(true);
    allButton_->setChecked(true);
    connect(allButton_, SIGNAL(clicked(bool)), this, SLOT(showAll_(bool)));

    errorButton_ = new QPushButton(tr("&Errors"));
    errorButton_->setCheckable(true);
    errorButton_->setChecked(false);
    connect(errorButton_, SIGNAL(clicked(bool)), this, SLOT(showErrors_(bool)));

    closeButton_ = new QPushButton(tr("&Close"));
    closeButton_->setEnabled(false);
    connect(closeButton_, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(allButton_);
    buttonLayout->addWidget(errorButton_);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton_);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(messageBrowser_);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

//-----------------------------------------------------------------------------

void
MessageWindow::closeEvent(QCloseEvent *event)
{
    if (!canClose_)
    {
        QMessageBox::warning(this, tr("Information"),
                             tr("Closing messsage window is prohibited by server!"),
                             QMessageBox::Ok);

        event->ignore();
    }
    else
    {
        event->accept();
        emit windowClosed(this);
    }
}

//-----------------------------------------------------------------------------

void
MessageWindow::closeWindow(quint16 delay, bool autoClose)
{
    autoClose_ = autoClose;
    canClose_ = true;

    if (delay > 0)
    {
        closeTimer_ = new QTimer(this);
        connect(closeTimer_, SIGNAL(timeout()), this, SLOT(closeWindow_()));

        closeTimer_->start(delay * 1000);
    }
}

//-----------------------------------------------------------------------------

void
MessageWindow::closeWindow_()
{
    if (autoClose_)
        close();
    else
        closeButton_->setEnabled(true);
}

//-----------------------------------------------------------------------------

void
MessageWindow::showAll_(bool state)
{
    if (state)
        errorButton_->setChecked(false);

    for (int row = 0; row < rowCount_; ++row)
        messageBrowser_->showRow(row);
}

//-----------------------------------------------------------------------------

void
MessageWindow::showErrors_(bool state)
{
    if (state)
    {
        allButton_->setChecked(false);

        for (int row = 0; row < rowCount_; ++row)
            if (messageBrowser_->item(row, 0)->type() != Core::MessageType::Critical)
                messageBrowser_->hideRow(row);
    }
}

//-----------------------------------------------------------------------------

void
MessageWindow::resizeEvent(QResizeEvent *event)
{
    messageBrowser_->setColumnWidth(0, messageBrowser_->width());

    QWidget::resizeEvent(event);
}


//-----------------------------------------------------------------------------

void
MessageWindow::showMessage(quint16 type, const QString &message)
{
    QTableWidgetItem* cell = new QTableWidgetItem(type);
    cell->setText(message);
    cell->setFont(QFont("Drouid Sans Mono Slashed", 10, QFont::DemiBold));

    switch (type)
    {
        case Core::MessageType::Info :
            cell->setTextColor(QColor(Qt::black));
        break;
        case Core::MessageType::Debug :
            cell->setTextColor(QColor(Qt::darkRed));
        break;
        case Core::MessageType::Critical :
            cell->setTextColor(QColor(Qt::red));
        break;
    }

    messageBrowser_->setRowCount(++rowCount_);

    messageBrowser_->setItem(rowCount_ - 1, 0, cell);
    messageBrowser_->setCurrentCell(rowCount_ - 1, 0, QItemSelectionModel::NoUpdate);
}

//-----------------------------------------------------------------------------

}
