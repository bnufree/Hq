#include "qandroidbutton.h"
#include <QDebug>
#include <QStyle>

QAndroidButton::QAndroidButton(const QString& label, QWidget *parent) :\
    QLabel(label,parent)
{
    mIsSelected = false;
    mRelatedWidget = 0;
    setMouseTracking(true);
    setAlignment(Qt::AlignCenter);
    this->setStyleSheet("\
                        QLabel[selected=\"true\"]\
                        {\
                            color:red;\
                            font-weight:bold;\
                        }\
                        QLabel[selected=\"false\"]\
                        {\
                            color:white;\
                            font-weight:normal;\
                        }\
                        ");
    setSelected(false);
}

void QAndroidButton::mousePressEvent(QMouseEvent *ev)
{
//    qDebug()<<__func__<<"!!!!!!!!!!!!!!!!!!!!!!!!!!";
    //this->setStyleSheet("color:red");
    emit clicked();
}

void QAndroidButton::leaveEvent(QEvent *ev)
{
//    qDebug()<<__func__<<"$$$$$$$$$$$$$$$$$$$$$$$$$$";
    //this->setStyleSheet("color:black");
//    this->setStyleSheet("background-color:transparent;font-weight:normal;");
}

void QAndroidButton::enterEvent(QEvent *ev)
{
//    qDebug()<<__func__<<"$$$$$$$$$$$$$$$$$$$$$$$$$$";
//    this->setStyleSheet("background-color:red;font-weight:bold;");
}

bool QAndroidButton::selected() const
{
    return mIsSelected;
}

void QAndroidButton::setSelected(bool sts)
{
    mIsSelected = sts;
    style()->polish(this);

}

