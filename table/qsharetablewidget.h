#ifndef QSHARETABLEWIDGET_H
#define QSHARETABLEWIDGET_H

#include "HqMergeTableWidget.h"
#include "data_structure/sharedata.h"
#include <QMutex>

class QSinaStkResultMergeThread;

class QShareTablewidget : public HqMergeTableWidget
{
    Q_OBJECT
public:
    explicit QShareTablewidget(QWidget *parent = 0);
public slots:
    void    setSelfShareCodesList(const QStringList& list);
private:
    void    initMenu();

signals:
    void    signalSetShareMarket(int mkt);
    void    signalSetDisplayMinuteGraph(const QString& code);
    void    signalSetDisplayDayGraph(const QString& code);
    void    signalSetDisplayBlockDetail(const QStringList& blockCodes);
    void    signalSetDisplayHSHK(const QString& code);
    void    signalSetFavCode(const QString& code);
    void    signalDisplayChinaTop10();
    void    signalSetSpecialConcern(const QString& code);
    void    signalDoubleClickCode(const QString& code);
    void    signalDisplayDetailOfCode(const QString& code);

public slots:
    void    updateTableInfo();
    void    setShareMarketType(int type);
    void    slotRecvAllShareDateList(const ShareDataList& list,qint64 time);
    void    setSortType(int type);
    void    slotCellDoubleClicked(int row, int col);
private:
    QMap<QString, double>   mShareMap;
    QList<QAction*>         mCodesActionList;
    ShareDataList                       mShareDataList;
    QMutex                              mDataMutex;
    QList<struMenu>                     mMktTypeList;
};

#endif // QSHARETABLEWIDGET_H
