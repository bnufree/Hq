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

void QIndexFrame::updateVal(double cur, double chg, double chgper, double money)
{
    ui->chg->setText(QString("").sprintf("%.0f", chg));
    ui->cur->setText(QString("").sprintf("%.0f", cur));
    ui->chgper->setText(QString("").sprintf("%.2f%", chgper));
    ui->money->setText(QStringLiteral("%1亿").arg(QString::number(money / 10000.0, 'f', 0)));
    int chgint = (int)(chg*100);
    this->setStyleSheet(QString("QLabel{"
                            "font-weight:bold;"
                            "color:%1;"
                            "alignment:center;"
                                "}")
                        .arg(chgint == 0? "black" : chgint > 0? "red":"green"));
}

void QIndexFrame::updateBound(double buy, double sell, double pure, double total)
{
    ui->cur->setText(QStringLiteral("买：%1亿").arg(QString::number(buy / 10000.0, 'f', 1)));
    ui->chg->setText(QStringLiteral("卖：%1亿").arg(QString::number(sell / 10000.0, 'f', 1)));
    ui->chgper->setText(QStringLiteral("净：%1亿").arg(QString::number(pure / 10000.0, 'f', 1)));
    ui->money->setText(QStringLiteral("总：%1亿").arg(QString::number(total / 10000.0, 'f', 0)));
    int chgint = (int)(pure);
    this->setStyleSheet(QString("QLabel{"
                            "font-weight:bold;"
                            "color:%1;"
                            "alignment:center;"
                                "}")
                        .arg(chgint == 0? "black" : chgint > 0? "red":"green"));
}
