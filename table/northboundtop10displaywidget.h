#ifndef NORTHBOUNDTOP10DISPLAYWIDGET_H
#define NORTHBOUNDTOP10DISPLAYWIDGET_H

#include "HqTableWidget.h"
#include "data_structure/sharedata.h"
#include <QThread>
#include <QLabel>

class QShareLgtTop10Thread :  public QThread
{
    Q_OBJECT
public:
    explicit QShareLgtTop10Thread( QObject *parent = 0);
    ~QShareLgtTop10Thread();
    void run();
private:
    bool getDataFromEastMoney(ShareHsgtList& list, QDate& date);
    bool getDataFromHKEX(ShareHsgtList& list, QDate& date);
signals:
    void signalChinaAShareTop10Updated(const ShareHsgtList& list, const QString& date);

private:
};


class NorthBoundTop10DisplayTable : public HqTableWidget
{
    Q_OBJECT
public:
    explicit NorthBoundTop10DisplayTable(QWidget *parent = nullptr);

signals:

public slots:
    void slotSetDataList(const ShareHsgtList &list);
    void slotCellDoubleClicked(int row, int col);
    void slotHeaderClicked(int col);

private:

    int         mSortCol;
    int         mSortMode;
    ShareHsgtList   mDataList;
};

class NorthBoundTop10DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NorthBoundTop10DisplayWidget(QWidget *parent = nullptr);

signals:

public slots:
    void slotSetDataList(const ShareHsgtList &list, const QString& code, const QDate& date);
private:
    QLabel      *mTimeLabel;
    NorthBoundTop10DisplayTable *mTable;
    QString     mCommonStr;

};

#endif // NORTHBOUNDTOP10DISPLAYWIDGET_H
