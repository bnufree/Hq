#ifndef QANDROIDLISTWIDGET_H
#define QANDROIDLISTWIDGET_H

#include "qandroidbutton.h"

class QAndroidListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QAndroidListWidget(QWidget *parent = 0);
    void     addItem(const QString& item, const QVariant& data);
protected:
    void     autoAdjustSize();
signals:
    void     signalItemClicked(int data);
public slots:
    void    slotItemClicked();
};

#endif // QANDROIDLISTWIDGET_H
