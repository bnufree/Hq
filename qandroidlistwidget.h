#ifndef QANDROIDLISTWIDGET_H
#define QANDROIDLISTWIDGET_H

#include "qandroidbutton.h"

class QAndroidListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QAndroidListWidget(int w = 0, int h = 0, QWidget *parent = 0);
    void     addItem(const QString& item, const QVariant& data);
protected:
    void     autoAdjustSize();
signals:
    void     signalItemClicked(const QString& text, int data);
public slots:
    void    slotItemClicked();
    void    slotFirstBtnClicked();
private:
    int     mItemWidth;
    int     mItemHeight;
    QList<QAndroidButton*>      mBtnList;
};

#endif // QANDROIDLISTWIDGET_H
