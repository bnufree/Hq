#ifndef ZCHXHQDISPLAYWIDGET_H
#define ZCHXHQDISPLAYWIDGET_H

#include <QWidget>
#include "qsharetablewidget.h"
#include "qblocktablewidget.h"

namespace Ui {
class zchxHqDisplayWidget;
}

class QAndroidButton;
class QAndroidListWidget;

class zchxHqDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit zchxHqDisplayWidget(QWidget *parent = 0);
    ~zchxHqDisplayWidget();
public slots:
    void slotMarketTypeChanged(const QString& text, int type);
    void slotMarketBtnClicked();

private:
    Ui::zchxHqDisplayWidget *ui;
    QShareTablewidget*       mShareWidget;
    QBlockTableWidget*       mBlockWidget;
    QAndroidButton*          mMarketBtn;
    QAndroidListWidget*      mMarketList;
};

#endif // ZCHXHQDISPLAYWIDGET_H
