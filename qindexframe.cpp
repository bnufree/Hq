#include "qindexframe.h"
#include "ui_qindexframe.h"

QIndexFrame::QIndexFrame(const QString& name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QIndexFrame)
{
    ui->setupUi(this);\
    ui->name->setText(name.trimmed());
}

QIndexFrame::~QIndexFrame()
{
    delete ui;
}

void QIndexFrame::setName(const QString &name)
{
    ui->name->setText(name);
}

void QIndexFrame::updateVal(double cur, double chg, double chgper)
{
    ui->chg->setText(QString("").sprintf("%.2f", chg));
    ui->cur->setText(QString("").sprintf("%.2f", cur));
    ui->chgper->setText(QString("").sprintf("%.2f%", chgper));
    int chgint = (int)(chg*100);
    this->setStyleSheet(QString("QLabel{"
                            "font-weight:bold;"
                            "color:%1;"
                            "alignment:center;"
                                "}")
                        .arg(chgint == 0? "black" : chgint > 0? "red":"green"));
}
