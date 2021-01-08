#ifndef HQTABLEWIDGET_H
#define HQTABLEWIDGET_H

#include <QTableWidget>
#include <QMenu>
#include "utils/comdatadefines.h"
#include <QResizeEvent>
#include <QGestureEvent>
#include <QDebug>

enum OPT_MOVE_MODE{
    OPT_LEFT = 0,
    OPT_RIGHT,
    OPT_UP,
    OPT_DOWN,
};

#define         HQ_NO_GESTURE

class HqTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit HqTableWidget(QWidget *parent = 0);
    bool isColVisible(int i) const {return !isColumnHidden(i);}
    bool isRowVisible(int i) const {return !isRowHidden(i);}
    void setTotalCount(int count) {mTotalDisplayRowCount = count;}
    void resetPageDisplay()
    {
        mDisplayRowEnd = mMaxDisRow-1;
        mDisplayRowStart = 0;
        updateTable();
    }
    void setHeaders(const TableColDataList& list);
    void appendRow();
    void setCodeName(int row, int column,const QString& code,const QString& name);
    void setItemText(int row, int column, const QString& text, const QColor& color = Qt::white, Qt::AlignmentFlag flg = Qt::AlignCenter);
    void setFavShareList(const QStringList& list);
    void appendFavShare(const QString& code);
    void removeFavShare(const QString& code);
    void updateFavShareIconOfRow(int row, bool isFav);
    void prepareUpdateTable(int newRowCount);
    void removeRows(int start, int count);
    void initPageCtrlMenu();
    QAction* insertContextMenu(QMenu* menu);
    void insertContextMenu(QAction *act);
    void updateColumn(int col);
    void setWorkInMini(bool sts);
    virtual void setSortType(int type) {}
    virtual void displayNextPage() {}
    virtual void displayPreviousPage() {}
    virtual void displayFirstPage(){}
    virtual void displayLastPage(){}
public slots:
    virtual void updateTable() {}
private:
protected:
    void resizeEvent(QResizeEvent *event);
    void displayVisibleCols();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool gestureEvent(QGestureEvent* event);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *event);
private slots:
    void checkColDisplayStatus();
    void checkRowDisplayStatus();
    void checkDisplayStatus();

signals:
    void signalDisplayPage(int val);
    void signalSetSortType(int type);
    void signalSetSortRule(int rule);
    void signalPageSizeChanged(int pagesize);

public slots:
    void slotSetDisplayPage();
    void slotSetColDisplay(bool isDisplay);
    virtual void slotCustomContextMenuRequested(const QPoint& pos);
    virtual void slotCellDoubleClicked(int row, int col);
    virtual void slotCellClicked(int row, int col);
    virtual void slotHeaderClicked(int col);
    void optMoveTable(OPT_MOVE_MODE mode, int step = 1);
    int  adjusVal(int val, int step, int max, int min);

private:
    TableColDataList        mColDataList;
    int                     mColWidth;
    int                     mRowHeight;
    QMenu                   *mCustomContextMenu;
    QStringList             mFavShareList;
    //int                     mOldRowCount;
    QScrollBar*             mCurScrollBar;
    QPoint                  mPressPnt;
    QPoint                  mMovePnt;
    int                     mMoveDir;
    int                     mMaxDisCol;
    bool                    mIsWorkInMini;
    quint64               mLastWheelTime;
    QList<int>              mColWidthList;
protected:
    int                     mDisplayRowStart;
    int                     mDisplayRowEnd;
    int                     mTotalDisplayRowCount;
    int                     mMaxDisRow;

};

//class HqDisTableWidget : public QWidget
//{
//    Q_OBJECT
//public:
//    explicit HqDisTableWidget(QWidget *parent = 0);
//    ~HqDisTableWidget();

//private:
//    QTableWidget*               mFixTable;
//    HqTableWidget*              mMoveTable;
//};

#endif // HqTableWidget_H
