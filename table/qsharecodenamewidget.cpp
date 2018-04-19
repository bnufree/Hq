#include "qsharecodenamewidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

QShareCodeNameWidget::QShareCodeNameWidget(const QString& code, const QString& name,QWidget *parent) :\
    QWidget(parent),\
    mCodeLbl(0),\
    mNameLbl(0)
{
    QVBoxLayout *vlay = new QVBoxLayout(this);
    vlay->setContentsMargins(0, 2, 0, 2);
    vlay->setSpacing(10);
    this->setLayout(vlay);
    mCodeLbl = new QLabel(code, this);
    mNameLbl = new QLabel(name, this);
    vlay->addWidget(mCodeLbl, 0,  Qt::AlignCenter);
    vlay->addWidget(mNameLbl, 0,  Qt::AlignCenter);
}

void QShareCodeNameWidget::setCode(const QString &str)
{
    if(mCodeLbl) mCodeLbl->setText(str);
}

void QShareCodeNameWidget::setName(const QString &str)
{
    if(mNameLbl) mNameLbl->setText(str);
}

