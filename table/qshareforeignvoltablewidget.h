#ifndef QSHAREFOREIGNVOLTABLEWIDGET_H
#define QSHAREFOREIGNVOLTABLEWIDGET_H

#include "HqMergeTableWidget.h"
#include "data_structure/sharedata.h"
#include <QThreadPool>

class QShareForeignVolChangeCounterThread;

class QShareForeignVolTableWidget : public HqMergeTableWidget
{
    Q_OBJECT
public:
    explicit QShareForeignVolTableWidget(QWidget *parent = 0);

signals:

public slots:
    void slotRecvData(const QList<ShareForeignVolCounter>& list, const QString& date);
    void updateTable();
    void    setSortType(int type);

private:
    QThreadPool         mPool;
    QList<ShareForeignVolCounter> mDataList;
    QShareForeignVolChangeCounterThread*    mDataThread;
};

#endif // QSHAREFOREIGNVOLTABLEWIDGET_H
