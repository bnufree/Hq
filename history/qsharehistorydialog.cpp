#include "qsharehistorydialog.h"
#include "ui_qsharehistorydialog.h"
#include "dbservices/dbservices.h"

QShareHistoryDialog::QShareHistoryDialog(const QString& code, QWidget *parent) :
    mCode(code),
    QDialog(parent),
    ui(new Ui::QShareHistoryDialog)
{
    ui->setupUi(this);
    //this->setFixedSize(200, 200);
    ui->widget->setValue(GRAPHIC_DATA_LIST(DATA_SERVICE->getShareHistoryDataList(mCode)));
}

QShareHistoryDialog::~QShareHistoryDialog()
{
    delete ui;
}
