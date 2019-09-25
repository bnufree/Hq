#include "qandroidbutton.h"
#include <QDebug>

QAndroidButton::QAndroidButton(const QString& label, QWidget *parent) :\
    QLabel(label,parent)
{
    setMouseTracking(true);
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

