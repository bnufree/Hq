#include "qsharehistorydialog.h"
#include "ui_qsharehistorydialog.h"

QShareHistoryDialog::QShareHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QShareHistoryDialog)
{
    ui->setupUi(this);
}

QShareHistoryDialog::~QShareHistoryDialog()
{
    delete ui;
}
