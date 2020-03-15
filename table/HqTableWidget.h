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
    void resetPageDisplay()
    {
        mDisRowEnd = mMaxDisRow-1;
        mDisRowStart = 0;
        displayVisibleRows();
    }

    void  resetDisplayRows(OPT_MOVE_MODE dir)
    {
        bool page_change = false;
        if(dir == OPT_DOWN)
        {
            mDisRowStart++;
            mDisRowEnd++;
        } else if(dir == OPT_UP)
        {
            mDisRowStart--;
            mDisRowEnd--;
        } else
        {
            return;
        }
        if(mDisRowEnd == rowCount())
        {
            page_change = true;
            displayNextPage();
        } else if(mDisRowStart == -1)
        {
            page_change = true;
            displayPreviousPage();
        }
        if(page_change)
        {
            mDisRowEnd = mMaxDisRow-1;
            mDisRowStart = 0;

        }
        qDebug()<<"row start:"<<mDisRowStart<<" end:"<<mDisRowEnd<<" page:"<<page_change;
        displayVisibleRows();
    }
    void setAutoChangePage(bool sts) {mAutoChangePage = sts;}
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
private:
protected:
    void resizeEvent(QResizeEvent *event);
    void displayVisibleCols();
    void displayVisibleRows();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool gestureEvent(QGestureEvent* event);
    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *event);
private slots:

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
    void slotHeaderClicked(int col);
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
    int                     mMaxDisRow;
    bool                    mAutoChangePage;
    bool                    mIsWorkInMini;
    quint64               mLastWheelTime;
protected:
    int                     mDisRowStart;
    int                     mDisRowEnd;
    int                     mCurPage;
    int                     mTotalPage;
    int                     mPageSize;

};

#endif // HqTableWidget_H
