#include "qindexframe.h"
#include "ui_qindexframe.h"
#include <QDebug>
#include "data_structure/hqutils.h"
#include <QStyle>
#include <QResizeEvent>

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
    setFixedSize(HqUtils::convertMM2Pixel(50.0), HqUtils::convertMM2Pixel(10.0));

}

QIndexFrame::~QIndexFrame()
{
    delete ui;
}

void QIndexFrame::resizeEvent(QResizeEvent *e)
{
    adjustFrameFont(e->size().width(), e->size().height());
}

void QIndexFrame::adjustFrameFont(int width, int height) const
{
    ui->horizontalLayout_2->setMargin(0);
    ui->verticalLayout->setMargin(0);
    ui->verticalLayout->setSpacing(2);
    int name_max_width = (width - 3 * ui->horizontalLayout_2->spacing()) / 4;
    QFont font = ui->name->font();
    while (1) {
        if(font.pixelSize() > 0)
            font.setPixelSize(font.pixelSize() + 1);
        else if(font.pointSize() > 0)
            font.setPixelSize(font.pointSize() + 1);
        else
            break;
        int cur_name_width =  QFontMetrics(font).width(tr("上证指数"));
        int cur_name_frame_height = QFontMetrics(font).height() / 0.9 * 2;
        if(cur_name_frame_height >= height || cur_name_width >= name_max_width)
        {
            break;
        }

    }

    ui->name->setFont(font);
    ui->cur->setFont(font);

    font = ui->money->font();
    while (1) {
        if(font.pixelSize() > 0)
            font.setPixelSize(font.pixelSize() + 1);
        else if(font.pointSize() > 0)
            font.setPixelSize(font.pointSize() + 1);
        else
            break;
        int cur_name_width =  QFontMetrics(font).width(tr("10000亿"));
        int cur_name_frame_height = QFontMetrics(font).height() / 0.9 * 2;
        if(cur_name_frame_height >= height || cur_name_width >= name_max_width)
        {
            break;
        }
    }

    ui->chg->setFont(font);
    ui->chgper->setFont(font);
    ui->money->setFont(font);
    frame_Width = width;
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
