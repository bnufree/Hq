#ifndef QANDROIDLISTWIDGET_H
#define QANDROIDLISTWIDGET_H

#include <QListWidget>

class QAndroidListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit QAndroidListWidget(QWidget *parent = 0);
    void     addItem(const QString& item);
protected:
    void     autoAdjustSize();
signals:

public slots:
    void    slotItemClicked(QListWidgetItem* item);
};

#endif // QANDROIDLISTWIDGET_H
