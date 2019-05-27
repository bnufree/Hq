#ifndef QCONTAINERWIDGET_H
#define QCONTAINERWIDGET_H

#include <QWidget>

enum LAYOUT_DIRECTION{
    LAYOUT_HOR,
    LAYOUT_VER,
};

class QContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QContainerWidget(LAYOUT_DIRECTION dir = LAYOUT_HOR, QWidget *parent = 0);
    bool appendWidget(QWidget* w);
    bool removeWidget(QWidget* w);
    void removeAllWidget();
    int  getWidgetCount();
    QWidget* getLastWidget();
    QWidget* getFirstWidget();
    QWidgetList getWidgetList();
    int  totalWidgetSize() const;

signals:

public slots:
private:
    LAYOUT_DIRECTION mDir;
};

#endif // QCONTAINERWIDGET_H
