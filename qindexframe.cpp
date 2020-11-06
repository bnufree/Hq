#include "qindexframe.h"
#include "ui_qindexframe.h"
#include <QDebug>
#include "data_structure/hqutils.h"
#include <QStyle>

int         frame_Width = 0;
QIndexFrame::QIndexFrame(const QString& name, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::QIndexFrame)
{
    mName = name;
    ui->setupUi(this);
    ui->name->setText(name.trimmed());
    ui->cur->clear();
    ui->chg->clear();
    ui->chgper->clear();
    ui->money->clear();
    setStatus(0);
    frame_Width = width();
}

QIndexFrame::~QIndexFrame()
{
    delete ui;
}


QSize QIndexFrame::calSize() const
{
    int half_height = HqUtils::convertMM2Pixel(4);
    QFont font = ui->name->font();
    HqUtils::setFontPixelSize(&font, half_height);
    font.setBold(false);
    ui->name->setFont(font);
    ui->cur->setFont(font);

    int total_height = 2* half_height + 4;
    int width = 18;
    //测试frame的宽度
    width += QFontMetrics(font).width(tr("上证指数"));
    HqUtils::setFontPixelSize(&font,total_height);
    ui->chg->setFont(font);
    ui->chgper->setFont(font);
    ui->money->setFont(font);
    width += QFontMetrics(font).width(tr("+300.00"));
    width += QFontMetrics(font).width(tr("+10.12%"));
    width += QFontMetrics(font).width(tr("10000亿"));
    width += 18;
    frame_Width = width;
    return QSize(width, total_height);
}


void QIndexFrame::setName(const QString &name)
{
    mName = name;
    ui->name->setText(name);
}

void QIndexFrame::updateData(const IndexFrameData& data)
{
//    qDebug()<<data.mName<<data.mCur<<data.mChg;
    if(data.mType == 1)
    {
        ui->name->setVisible(true);
        ui->cur->setVisible(true);
        ui->name->setText(data.mName);
        updateVal(data.mCur, data.mChg, data.mChgPer, data.mTotal);
    } else
    {
        updateBound(data.mCur, data.mName);
    }
}


void QIndexFrame::updateVal(double cur, double chg, double chgper, double money)
{
    ui->chg->setText(QString("").sprintf("%.2f", chg));
    ui->cur->setText(QString("").sprintf("%.2f", cur));
    ui->chgper->setText(QString("").sprintf("%.2f%", chgper));
    ui->money->setText(QStringLiteral("%1亿").arg(QString::number(money / 100000000.0, 'f', 0)));
    int chgint = (int)(chg*100);
    int sts = (chgint == 0? 0 : chgint > 0? 1: -1);
    setStatus(sts);
}

void QIndexFrame::updateBound(double shVal, QString shName, double szVal, QString szName)
{
    ui->cur->setText(shName);
    ui->chg->setText(QStringLiteral("%1亿").arg(QString::number(shVal / 10000.0, 'f', 2)));
    ui->chgper->setText(szName);
    ui->money->setText(QStringLiteral("%1亿").arg(QString::number(szVal / 10000.0, 'f', 2)));
    int chgint = (int)(shVal+szVal);
    int sts = (chgint == 0? 0 : chgint > 0? 1: -1);
    setStatus(sts);
}

void QIndexFrame::updateBound(double pure, const QString &name)
{
    ui->name->setText(name);
    ui->name->setVisible(false);
    ui->cur->setText(name);
    ui->cur->setVisible(false);
    ui->chg->setText(name);
    ui->chgper->setText(QStringLiteral("%1亿").arg(QString::number(pure / 10000.0, 'f', 2)));
    ui->money->setVisible(false);
    int chgint = (int)(pure);
    int sts = (chgint == 0? 0 : chgint > 0? 1: -1);
    setStatus(sts);
}

void QIndexFrame::updateBound(double buy, double sell, double pure, double total)
{
    ui->cur->setText(QStringLiteral("买：%1亿").arg(QString::number(buy / 10000.0, 'f', 1)));
    ui->chg->setText(QStringLiteral("卖：%1亿").arg(QString::number(sell / 10000.0, 'f', 1)));
    ui->chgper->setText(QStringLiteral("净：%1亿").arg(QString::number(pure / 10000.0, 'f', 1)));
    ui->money->setText(QStringLiteral("总：%1亿").arg(QString::number(total / 10000.0, 'f', 0)));
    int chgint = (int)(pure);
    int sts = (chgint == 0? 0 : chgint > 0? 1: -1);
    setStatus(sts);
}

int QIndexFrame::status() const
{
    return mStatus;
}

void QIndexFrame::setStatus(int sts)
{
    mStatus = sts;
    QLayout* pLayout = layout();
    if(!pLayout) return;
    setLayoutLabel(pLayout, sts);
}

void QIndexFrame::setLayoutLabel(QLayout *pLayout, int sts)
{
    if(!pLayout) return;
    int itemCount = pLayout->count();
    for(int i=0; i<itemCount; i++)
    {
        QLayoutItem* item = pLayout->itemAt(i);
        if(!item) continue;
        QLayout *layout = item->layout();
        if(layout)
        {
            setLayoutLabel(layout, sts);
            continue;
        }
        QWidget* widget = item->widget();
        if(!widget) continue;
        ColorTextLabel* label = qobject_cast<ColorTextLabel*>(widget);
        if(label)label->setTextColor(sts);
    }
}
