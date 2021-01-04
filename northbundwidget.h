#ifndef NORTHBUNDWIDGET_H
#define NORTHBUNDWIDGET_H

#include <QStackedWidget>

class QAndroidButton;
class QAndroidListWidget;

enum NorthType{
    North_RealTime = 0,
    North_Top10,
    North_Vol,
};

class NorthBundWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NorthBundWidget(QWidget *parent = 0);

signals:

public slots:
    void slotMarketBtnClicked();
    void slotMarketTypeChanged(const QString& text, int type);
private:
    QAndroidButton*         mCtrlBtn;
    QAndroidListWidget*     mTypeList;
    QStackedWidget*         mWidgetList;
};

#endif // NORTHBUNDWIDGET_H
