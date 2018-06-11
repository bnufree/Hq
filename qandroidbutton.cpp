#include "qandroidbutton.h"
#include <QDebug>

QAndroidButton::QAndroidButton(const QString& label, QWidget *parent) :\
    QLabel(label,parent)
{

}

void QAndroidButton::mousePressEvent(QMouseEvent *ev)
{
//    qDebug()<<__func__<<"!!!!!!!!!!!!!!!!!!!!!!!!!!";
    //this->setStyleSheet("color:red");
    emit clicked();
}

void QAndroidButton::focusOutEvent(QFocusEvent *ev)
{
//    qDebug()<<__func__<<"$$$$$$$$$$$$$$$$$$$$$$$$$$";
    //this->setStyleSheet("color:black");
}

