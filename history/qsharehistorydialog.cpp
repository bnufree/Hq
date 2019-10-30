#include "qsharehistorydialog.h"
#include "ui_qsharehistorydialog.h"
#include "dbservices/dbservices.h"

QShareHistoryDialog::QShareHistoryDialog(const QString& code, QWidget *parent) :
    mCode(code),
    QDialog(parent),
    ui(new Ui::QShareHistoryDialog)
{
    ui->setupUi(this);
    setWindowTitle(mCode);    
    ui->widget->setTitle(mCode);
    ui->widget->setCode(mCode);
}

QShareHistoryDialog::~QShareHistoryDialog()
{
    delete ui;
}
