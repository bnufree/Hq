#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QTableWidget>
#include "qeastmoneyblockmangagerthread.h"
#include "qsinastkinfothread.h"
#include "qsinasearchthread.h"
#include "qsinastkresultmergethread.h"

typedef struct strColDisplay
{
    QTableWidget*   mTable;
    int             mColIndex;
    bool            mIsDisplay;
}TableColDisplayStatus;


Q_DECLARE_METATYPE(TableColDisplayStatus)

namespace Ui {
class MainDialog;
}

enum{
    MENU_OPT_MINUTE,
    MENU_OPT_DAY,    
    MENU_OPT_HSGT,
    MENU_OPT_BLOCK,
};

typedef enum Display_Mode{
    E_DISPLAY_ALL = 0,
    E_DISPLAY_BLOCK = 1,
    E_DISPLAY_STOCK_FULL,
    E_DISPLAY_STOCK_MINI,
}DISPLAY_MODE;

struct HqTableMenuData
{
    QString mStockCode;
    QString mBlockCode;
    int     mMenuCmd;
};

Q_DECLARE_METATYPE(HqTableMenuData)

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    //static void HQLISIINFOCBKFUNC(StockDataList& pDataList, void *pUser);
    //static void HQBLOCKINFOCBKFUNC(BlockDataList& pDataList, void *pUser);


protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    //bool eventFilter(QObject *obj, QEvent *event);
    //void keyPressEvent(QKeyEvent *);
public slots:
    void setSortType(int index);
    void setBlockSort(int val);
    void setBlockName();
    void setStockMarket();
    void setDisplayPage();
    void setDisplayCol(bool isDisplay);
    void setTargetSize(const QSize& size);

    void updateBlockTable(const BlockDataList& pDataList);
    void displayBlockRealtimeInfo();
    void updateHqTable(const StockDataList& pDataList);
    void displayBlockDetailInfoInTable(const QStringList& stklist);
    void editFavorite();
    void setDlgShow(QSystemTrayIcon::ActivationReason val);
    void slotUpdate(int cur, int total);
    void hqMenuOpt();
    void slotUpdateMsg(const QString& msg);
    void slotUpdateIndex(const StockDataList& pDataList);
    void slotSystemTrayMenuClicked();
    void slotWhetherDisplay();
    void slotRestartMyself();
    void slotDisplayAll();
    void slotDisplayBlock();
    void slotDisplayStockFull();
    void slotDisplayStockMini();
    void slotUpdateStockCodesList(const QStringList& list);
    void slotTodayHSGUpdated();
    void slotHistoryDataFinish();

private slots:
    void on_zxgBtn_clicked();

    void iniHqCenterAction();

    void on_blkbtn_clicked();

    void on_zjlxBtn_clicked();

    void on_lhbBtn_clicked();

    void on_closeBtn_clicked();

    void on_srchBtn_clicked();

    void on_minBtn_clicked();

    void on_blocktbl_itemDoubleClicked(QTableWidgetItem *item);

    void on_blocktbl_customContextMenuRequested(const QPoint &pos);

    void on_hqtbl_customContextMenuRequested(const QPoint &pos);

    void on_searchTxt_textChanged(const QString &arg1);

    void on_DateMgrBtn_clicked();

    void on_hqtbl_itemDoubleClicked(QTableWidgetItem *item);

    void on_hqtbl_itemEntered(QTableWidgetItem *item);

    void on_MainDialog_customContextMenuRequested(const QPoint &pos);

    void on_hqtbl_itemClicked(QTableWidgetItem *item);

    void on_blocktbl_itemClicked(QTableWidgetItem *item);

    void on_HSGTBTN_clicked();

private:
    Ui::MainDialog *ui;
    QSystemTrayIcon *systemIcon;
    QSinaStkInfoThread            *mIndexThread;
    QEastMoneyBlockMangagerThread        *mBlockMgr;
    QSinaStkResultMergeThread     *mMergeThread;
    QSinaSearchThread             *mSearchThread;
    QMap<QString, double> mStockMap;
    QMap<QString, double> mBlockMap;
    QMap<QString, QString> mBlockNameMap;
    QMap<QString, QStringList> mBlockStkList;
    QStringList           mFavStkList;
    QStringList           mHSFoundsList;
    QStringList           mAllStkList;
    int                   mCurBlockType;
    int                   mDisplayMode;
    int                   mSecSize;
    bool                  mInit;
    QSize                 mTargetSize;
    QMenu                 *mHqCenterMenu;
    QMenu                 *mHqHeaderMenu;
    QMenu                 *mHqPageMenu;
    QStringList           mHqHeaderList;
    int                   mDisplayCol;
    QList<QAction*>       mHqColActList;
    QPoint   windowPos;
    QPoint   mousePos;
    QPoint   dPos;
};

#endif // DIALOG_H
