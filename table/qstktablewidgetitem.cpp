#include "qstktablewidgetitem.h"

QStkTableWidgetItem::QStkTableWidgetItem():QTableWidgetItem()
{
    setTextAlignment(Qt::AlignCenter);
}

QStkTableWidgetItem::QStkTableWidgetItem(const QString &text, Qt::AlignmentFlag flg)
                    :QTableWidgetItem(text)
{
    setTextAlignment(flg);
}

QStkTableWidgetItem::QStkTableWidgetItem(double val, Qt::AlignmentFlag flg)
                    :QTableWidgetItem(QString::number(val, 'f', 2))
{
    setTextAlignment(flg);
}

QStkTableWidgetItem::QStkTableWidgetItem(int val, Qt::AlignmentFlag flg)
                    :QTableWidgetItem(QString::number(val))
{
    setTextAlignment(flg);
}

QStkTableWidgetItem::~QStkTableWidgetItem()
{

}

void QStkTableWidgetItem::setString(const QString &str)
{
    setText(str);
}

void QStkTableWidgetItem::setInt(int val)
{
    setText(QString::number(val));
}

void QStkTableWidgetItem::setDouble(int val)
{
    setText(QString::number(val, 'f', 2));
}
