#ifndef QINDEXWIDGET_H
#define QINDEXWIDGET_H

#include <QWidget>
#include "stockdata.h"
#include "qindexframe.h"
#include <QMap>

class QIndexWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QIndexWidget(QWidget *parent = 0);
    ~QIndexWidget();


signals:

public slots:
    void updateData(const StockDataList& list);

private:
    QMap<QString, QIndexFrame*> mIndexWidgetMap;
};

#endif // QINDEXWIDGET_H
