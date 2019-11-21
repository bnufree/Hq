#include "qsharecodenamewidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QResizeEvent>
#include "data_structure/hqutils.h"

QShareCodeNameWidget::QShareCodeNameWidget(const QString& code, const QString& name,QWidget *parent) :\
    QWidget(parent),\
    mCodeLbl(0),\
    mNameLbl(0)
{
    this->setStyleSheet("QLabel{font-family:Microsoft Yahei;font-weight:normal; color:white;}");
//    this->setFont(font);
    QVBoxLayout *vlay = new QVBoxLayout(this);
    vlay->setContentsMargins(0, 1, 0, 1);
    vlay->setSpacing(1);
    this->setLayout(vlay);
    mCodeLbl = new QLabel(code, this);
    mNameLbl = new QLabel(name, this);
    vlay->addWidget(mNameLbl, 0,  Qt::AlignCenter);
    vlay->addWidget(mCodeLbl, 0,  Qt::AlignCenter);
}

void QShareCodeNameWidget::setCode(const QString &str)
{
    if(mCodeLbl)
    {
        mCodeLbl->setText(str);
        updateDisplay();
    }
}

void QShareCodeNameWidget::setName(const QString &str)
{
    if(mNameLbl)
    {
        mNameLbl->setText(str);
        updateDisplay();
    }
}

void QShareCodeNameWidget::setFavour(bool sts)
{
    QString color_style = QString("color:%1;").arg(sts ? "magenta" : "white");
    QString style = this->styleSheet();
    int index = style.indexOf("color:");
    if(index < 0) return;
    int end_index = style.indexOf(";", index+1);
    style.replace(index, end_index - index + 1, color_style);

    this->setStyleSheet(style);
}

void QShareCodeNameWidget::resizeEvent(QResizeEvent *e)
{

    QWidget::resizeEvent(e);
    updateDisplay();
}

void QShareCodeNameWidget::updateDisplay()
{
    QSize size = this->size();
    if(size.height() == 0) return;
    int height = size.height() - 3;
    int code_h = qRound(height * 0.4);
    int name_h = height - code_h;
    QFont font1 = mCodeLbl->font();
    HqUtils::setFontPixelSize(&font1, code_h*0.8);
    QFont font2 = mNameLbl->font();
    HqUtils::setFontPixelSize(&font2, name_h*0.8);
    int name_width = size.width() * 0.8;
    if(QFontMetrics(font2).width(tr("天长地久")) > name_width)
    {
        HqUtils::setFontPixelSize(&font2, font2.pixelSize() * 0.8);
    }
    mCodeLbl->setFont(font1);
    mNameLbl->setFont(font2);
}

