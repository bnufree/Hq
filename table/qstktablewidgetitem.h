#ifndef QSTKTABLEWIDGETITEM_H
#define QSTKTABLEWIDGETITEM_H

#include <QTableWidgetItem>

class QStkTableWidgetItem : public QTableWidgetItem
{
public:
    QStkTableWidgetItem();
    QStkTableWidgetItem(const QString& text, Qt::AlignmentFlag flg = Qt::AlignCenter);
    QStkTableWidgetItem(double val, Qt::AlignmentFlag flg = Qt::AlignCenter);
    QStkTableWidgetItem(int val, Qt::AlignmentFlag flg = Qt::AlignCenter);
    void setString(const QString& str);
    void setInt(int val);
    void setDouble(int val);

    ~QStkTableWidgetItem();
};

#endif // QSTKTABLEWIDGETITEM_H
