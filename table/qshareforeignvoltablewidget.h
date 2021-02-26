#ifndef QSHAREFOREIGNVOLTABLEWIDGET_H
#define QSHAREFOREIGNVOLTABLEWIDGET_H

#include "HqMergeTableWidget.h"
#include "data_structure/sharedata.h"
#include <QThreadPool>
#include <QLabel>

class QShareForeignVolChangeCounterThread;

class QShareForeignVolTableWidget : public HqMergeTableWidget
{
    Q_OBJECT
public:
    explicit QShareForeignVolTableWidget(QShareForeignVolChangeCounterThread* thread, QWidget *parent = 0);

signals:

public slots:
    void slotRecvData(const QList<ShareForeignVolCounter>& list);
    void updateTableInfo();
    void    setSortType(int type);

private:
    QList<ShareForeignVolCounter> mDataList;
};

class LGTVolDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LGTVolDisplayWidget(QWidget *parent = nullptr);

signals:

public slots:
    void slotRecvData(const QList<ShareForeignVolCounter>& list, const QString& date);
private:
    QLabel      *mTimeLabel;
    QShareForeignVolTableWidget *mTable;
    QString     mCommonStr;

};

#endif // QSHAREFOREIGNVOLTABLEWIDGET_H
