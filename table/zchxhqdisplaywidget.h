#ifndef ZCHXHQDISPLAYWIDGET_H
#define ZCHXHQDISPLAYWIDGET_H

#include <QWidget>
#include "qsharetablewidget.h"
#include "qblocktablewidget.h"

namespace Ui {
class zchxHqDisplayWidget;
}

class zchxHqDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit zchxHqDisplayWidget(QWidget *parent = 0);
    ~zchxHqDisplayWidget();
public slots:
    void slotMarketTypeChanged(int);

private:
    Ui::zchxHqDisplayWidget *ui;
    QShareTablewidget*       mShareWidget;
    QBlockTableWidget*       mBlockWidget;
};

#endif // ZCHXHQDISPLAYWIDGET_H
