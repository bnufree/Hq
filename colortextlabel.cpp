#include "colortextlabel.h"
#include <QStyle>
#include <QDebug>

ColorTextLabel::ColorTextLabel(QWidget *parent) : QLabel(parent)
{
//    QLabel{\
//                      font-family:Microsoft Yahei;\
//                      font-size:12pt;\
//                      font-weight:bold;\
//                      border:none;\
//                      color:darkgray;\
//                      }\

    setStyleSheet("QLabel[textColor=\"0\"]{color:white;}\
QLabel[textColor=\"1\"]{color:red;}\
QLabel[textColor=\"-1\"]{color:green;}");

setTextColor(0);
}

int ColorTextLabel::textColor() const
{
    return mTextColor;
}

void ColorTextLabel::setTextColor(int sts)
{
    mTextColor = sts;
//    qDebug()<<"update label:"<<mTextColor;
    style()->polish(this);
}
