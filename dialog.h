#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QTableWidget>
#include "data_structure/hqbasedata.h"
#include "data_structure/sharedata.h"
#include "table/qsharetablewidget.h"
#include "table/qblocktablewidget.h"
#include "qdatamgrwidget.h"

class HQTaskMagrCenter;
class QIndexWidget;
class QShareForeignVolTableWidget;
class QAndroidListWidget;

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
    E_DISPLAY_Share_FULL,
    E_DISPLAY_Share_MINI,
}DISPLAY_MODE;

struct HqTableMenuData
{
    QString mShareCode;
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
    //static void HQLISIINFOCBKFUNC(ShareDataList& pDataList, void *pUser);
    //static void HQBLOCKINFOCBKFUNC(BlockDataList& pDataList, void *pUser);


protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    //bool eventFilter(QObject *obj, QEvent *event);
    //void keyPressEvent(QKeyEvent *);
public slots:
    void setTargetSize(const QSize& size);
    void slotRecvIndexCodesList(const QStringList& list);
    void updateBlockTable(const BlockDataList& pDataList);
    void updateHqTable(const ShareDataList& pDataList);
    void setDlgShow(QSystemTrayIcon::ActivationReason val);
    void slotUpdateMsg(const QString& msg);
    void slotSystemTrayMenuClicked();
    void slotWhetherDisplay();
    void slotRestartMyself();
    void slotDisplayAll();
    void slotDisplayBlock();
    void slotDisplayShareFull();
    void slotDisplayShareMini();
    void slotTodayHSGUpdated();
    void slotUpdateHSGTOfCode(const QString& code);
    void slotHqCenterBtnClicked();
    void slotDisplayDataPage(int val);
    void slotDisplayHqCenterPage(int val);
    void slotSetSpecialConcern(const QString& code);
    void showMini();
    void showMax();
    void slotDoubClickedCode(const QString& code);
private slots:
    void on_zjlxBtn_clicked();

    void on_lhbBtn_clicked();

    void on_closeBtn_clicked();

    void on_srchBtn_clicked();

    void on_minBtn_clicked();

    void on_searchTxt_textChanged(const QString &arg1);

    void on_DateMgrBtn_clicked();

    void on_MainDialog_customContextMenuRequested(const QPoint &pos);

    void on_HSGTBTN_clicked();

    void on_dataMgrBtn_clicked();

    void on_min_clicked();

    void on_max_clicked();

    void on_close_clicked();

private:
    Ui::MainDialog *ui;
    QSystemTrayIcon       *systemIcon;
    int                   mDisplayMode;
    int                   mSecSize;
    QSize                 mTargetSize;
//    QMenu                 *mHqCenterMenu;
//    QMenu                 *mHqHeaderMenu;
//    QMenu                 *mHqPageMenu;
//    QStringList           mHqHeaderList;
    int                   mDisplayCol;
    QList<QAction*>       mHqColActList;
    QPoint   windowPos;
    QPoint   mousePos;
    QPoint   dPos;
    HQTaskMagrCenter     *mTaskMgr;
    QIndexWidget        *mIndexWidget;
    QShareTablewidget   *mShareTableWidget;
    QBlockTableWidget   *mBlockTableWidget;
    QDataMgrWidget      *mDataMgrWidget;
    QShareForeignVolTableWidget *mForeignVolTableWidget;
    bool                mIsMini;
    QAndroidListWidget  *mCtrlListWidget;
    QList<QWidget*>             mListWidget;
};

#endif // DIALOG_H
