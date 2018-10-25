#ifndef HQTABLEWIDGET_H
#define HQTABLEWIDGET_H

#include <QTableWidget>
#include <QMenu>
#include "utils/comdatadefines.h"
#include <QResizeEvent>
#include <QGestureEvent>

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
private:
protected:
    void resizeEvent(QResizeEvent *event);
    void displayVisibleCols();
    void displayVisibleRows();
#ifdef HQ_NO_GESTURE
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
#else
    bool event(QEvent *e);
    bool gestureEvent(QGestureEvent* event);
#endif
private slots:

signals:
    void signalDisplayPage(int val);
    void signalSetSortType(int type);
    void signalSetSortRule(int rule);

public slots:
    void slotSetDisplayPage();
    void slotSetColDisplay(bool isDisplay);
    virtual void slotCustomContextMenuRequested(const QPoint& pos);
    virtual void slotCellDoubleClicked(int row, int col);
    void slotCellClicked(int row, int col);
    void slotHeaderClicked(int col);
    void optMoveTable(OPT_MOVE_MODE mode);

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
    int                     mMaxDisplayCol;
    int                     mMaxDisplayRow;
};

#endif // HqTableWidget_H
