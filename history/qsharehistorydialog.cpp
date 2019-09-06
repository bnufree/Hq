#include "qsharehistorydialog.h"
#include "ui_qsharehistorydialog.h"
#include "dbservices/dbservices.h"
#include "sharehistoryreadthread.h"

QShareHistoryDialog::QShareHistoryDialog(const QString& code, QWidget *parent) :
    mCode(code),
    QDialog(parent),
    ui(new Ui::QShareHistoryDialog)
{
    ui->setupUi(this);
    qRegisterMetaType<GRAPHIC_DATA>("const GRAPHIC_DATA&");
    qRegisterMetaType<GRAPHIC_DATA_LIST>("const GRAPHIC_DATA_LIST&");
    ShareHistoryReadThread *thread = new ShareHistoryReadThread(code);
    connect(thread, SIGNAL(signalSendGraphicDataList(GRAPHIC_DATA_LIST)), ui->widget, SLOT(setValue(GRAPHIC_DATA_LIST)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    ui->widget->setTitle(mCode);

    //this->setFixedSize(200, 200);
    //ui->widget->setValue(GRAPHIC_DATA_LIST(DATA_SERVICE->getShareHistoryDataList(mCode)));
}

QShareHistoryDialog::~QShareHistoryDialog()
{
    delete ui;
}
