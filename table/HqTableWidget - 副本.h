#ifndef HQTABLEWIDGET_H
#define HQTABLEWIDGET_H

#include <QTableWidget>
#include <QMenu>
#include "utils/comdatadefines.h"
#include <QResizeEvent>
#include <QGestureEvent>

class HqTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit HqTableWidget(QWidget *parent = 0);
    void setHeaders(const TableColDataList& list);
    void appendRow();
    void setItemText(int row, int column, const QString& text, Qt::AlignmentFlag flg = Qt::AlignCenter);
    void setFavShareList(const QStringList& list);
    void appendFavShare(const QString& code);
    void removeFavShare(const QString& code);
    void updateFavShareIconOfRow(int row, bool isFav);
    void prepareUpdateTable(int newRowCount);
    void removeRows(int start, int count);
    void initPageCtrlMenu();
    QAction* insertContextMenu(QMenu* menu);
    void insertContextMenu(QAction *act);
protected:
    bool event(QEvent *event);
    void resizeEvent(QResizeEvent *event);
    bool gestureEvent(QGestureEvent* event);
    //bool eventFilter(QObject *obj, QEvent *event);
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

private:
    TableColDataList        mColDataList;
    int                     mColWidth;
    QMenu                   *mCustomContextMenu;
    QStringList             mFavShareList;
    QScrollBar*             mCurScrollBar;
    //int                     mOldRowCount;
    QPoint                  mPressPnt;
    QPoint                  mMovePnt;
    int                     mMoveDir;
};

#endif // HqTableWidget_H
