#include "HqTableWidget.h"
#include "utils/comdatadefines.h"
#include <QHeaderView>
#include <QAction>
#include "qstktablewidgetitem.h"
#include <QDebug>
#include <QScrollBar>
#include <QMessageBox>
#include "qsharecodenamewidget.h"
#include <math.h>
#include <QDesktopWidget>

#define     COL_TYPE_ROLE               Qt::UserRole + 1
#define     COL_SORT_ROLE               Qt::UserRole + 2
#define     PAGE_SIZE                   50

HqTableWidget::HqTableWidget(QWidget *parent) : QTableWidget(parent),\
    mCurScrollBar(0),
    mMoveDir(-1),
    mCustomContextMenu(0),
    mDisplayRowStart(0),
    mDisplayRowEnd(0)
{
    this->setItemDelegate(new RowDelegate);
    this->setRowCount(PAGE_SIZE);
    initPageCtrlMenu();
    mColDataList.clear();
//    mColWidth = 60;
    this->verticalHeader()->setVisible(false);
//    this->horizontalHeader()->setDefaultSectionSize(mColWidth);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //鼠标右键选择
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slotCellDoubleClicked(int,int)));
    this->horizontalHeader()->setHighlightSections(false);
    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));
    //this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->horizontalScrollBar()->setPageStep(1);
    this->verticalScrollBar()->setPageStep(1);
#ifndef HQ_NO_GESTURE
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::TapAndHoldGesture);
#endif
    //根据当前屏幕的大小来设定显示的行高和列宽
    QRect rect = QApplication::desktop()->availableGeometry();
    QFont font = this->font();
    font.setBold(false);
    font.setPointSize(20);
    this->setFont(font);
    //默认屏幕大小为1920*1080
    int font_height1 = this->fontMetrics().height();
    mRowHeight = (/*1080.0 * 1.0 / rect.height() * 40*/this->fontMetrics().height()* 2);

    qDebug()<<"height:"<<font_height1<<rect.height()<<mRowHeight;
    mColWidth = qRound(1920.0 * 1.0 / rect.width() * 140);
    //设定item的大小
    //this->setStyleSheet(QString("QTableview::item{height:%1;font-weight:bold;font-size:20pt;}").arg(mRowHeight));
}

void HqTableWidget::setHeaders(const TableColDataList &list)
{
    QMenu *menu = new QMenu(QStringLiteral("列表标题"), this);
    mColDataList = list;
    this->setColumnCount(list.length());
    for(int i=0; i<mColDataList.size(); i++) {
        this->setColumnWidth(i, 80);
        mColDataList[i].mColNum = i;
        this->setHorizontalHeaderItem(i, new QStkTableWidgetItem(mColDataList[i].mColStr));
        this->horizontalHeaderItem(i)->setData(COL_TYPE_ROLE, mColDataList[i].mType);
        this->horizontalHeaderItem(i)->setData(COL_SORT_ROLE, QVariant::fromValue((void*) &(mColDataList[i].mRule)));
        QAction *act = new QAction(this);
        act->setText(this->horizontalHeaderItem(i)->text());
        act->setData(QVariant::fromValue((void*) &(mColDataList[i])));
        act->setCheckable(true);
        act->setChecked(mColDataList[i].mIsDisplay);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(slotSetColDisplay(bool)));
        menu->addAction(act);
        if(!mColDataList[i].mIsDisplay)
        {
            this->setColumnHidden(i, true);
        }
    }

    insertContextMenu(menu);
}

void HqTableWidget::slotHeaderClicked(int col)
{
    emit signalSetSortType(this->horizontalHeaderItem(col)->data(COL_TYPE_ROLE).toInt());
    resetDisplayRows();
}

void HqTableWidget::slotSetColDisplay(bool isDisplay)
{
    Q_UNUSED(isDisplay)
    QAction *act = (QAction*) sender();
    if(!act) return;
    TableColData *data = (TableColData*)(act->data().value<void*>());
    if(!data) return;
    this->setColumnHidden(data->mColNum, data->mIsDisplay);
    data->mIsDisplay = !(data->mIsDisplay);
}

void HqTableWidget::appendRow()
{
    this->insertRow(this->rowCount());
}

void HqTableWidget::setItemText(int row, int column, const QString &text, const QColor& color, Qt::AlignmentFlag flg)
{
    QStkTableWidgetItem *item = static_cast<QStkTableWidgetItem*> (this->item(row, column));
    if(!item)
    {
        item = new QStkTableWidgetItem(text, flg);
        setItem(row, column, item);
    }

    item->setString(text);
    item->setTextColor(color);
    QFont font = this->font();
    if(!this->isColumnHidden(column))
    {
        //字体自适应
        int text_width = QFontMetrics(font).width(text);
        while (text_width > this->columnWidth(column) - 10 && font.pointSize() > 1) {
            font.setPointSize(font.pointSize()-1);
            text_width = QFontMetrics(font).width(text);
        }
    }
    item->setFont(font);
}

void HqTableWidget::updateColumn(int col)
{
    for(int i=0; i<this->rowCount();i++)
    {
        if(this->isRowHidden(i)) continue;
        QTableWidgetItem *item = this->item(i, col);
        if(!item) continue;
        if(item->text().trimmed().length() == 0) continue;
        QString text = item->text();
        QFont font = item->font();
        //字体自适应
        int text_width = QFontMetrics(font).width(text);
        while (text_width > this->columnWidth(col) - 10 && font.pointSize() > 1) {
            font.setPointSize(font.pointSize()-1);
            text_width = QFontMetrics(font).width(text);
        }
        item->setFont(font);
    }
}



void HqTableWidget::setCodeName(int row, int column, const QString &code, const QString &name)
{
    QShareCodeNameWidget *item = static_cast<QShareCodeNameWidget*> (this->cellWidget(row, column));
    if(item)
    {
        item->setCode(code);
        item->setName(name);
    }
    else
    {
        item = new QShareCodeNameWidget(code, name);
        this->setCellWidget(row, column, item);
    }
    setItemText(row, column, "");
}

void HqTableWidget::setFavShareList(const QStringList &list)
{
    mFavShareList = list;
}

void HqTableWidget::appendFavShare(const QString &code)
{
    if(!mFavShareList.contains(code)) mFavShareList.append(code);
}

void HqTableWidget::removeFavShare(const QString &code)
{
    if(mFavShareList.contains(code)) mFavShareList.removeOne(code);
}

void HqTableWidget::updateFavShareIconOfRow(int row, bool isFav)
{
    if(row >= this->rowCount()) return;
    QShareCodeNameWidget * w = static_cast<QShareCodeNameWidget*>(cellWidget(row, 0));
    if(!w) return;
    QString color = isFav ? "magenta" : "white";
    w->setStyleSheet(QString("color:%1").arg(color));
}

void HqTableWidget::prepareUpdateTable(int newRowCount)
{
    int oldRowCount = this->rowCount();
    if(oldRowCount < newRowCount)
    {
        //this->setRowCount(newRowCount);
    } else if(oldRowCount > newRowCount)
    {
        removeRows(newRowCount, oldRowCount - newRowCount);
    } else
    {
        //do nothing
    }
    this->setRowCount(newRowCount);
    for(int i=0; i<this->rowCount(); i++)
    {
        this->setRowHeight(i, mRowHeight);
    }
}

void HqTableWidget::removeRows(int start, int count)
{
    for(int i=0; i<count; i++)
    {
        int row = start + i;
        for(int k=0; k<this->columnCount(); k++)
        {
            QShareCodeNameWidget *w = static_cast<QShareCodeNameWidget*> (this->cellWidget(row, k));
            if(w)
            {
                delete w;
                w = 0;
            }

            QStkTableWidgetItem *item = (QStkTableWidgetItem*)(this->item(row, k));
            if(item)
            {
                delete item;
                item = 0;
            }
        }
    }
}

void HqTableWidget::slotCustomContextMenuRequested(const QPoint &pos)
{
    qDebug()<<__FILE__<<__FUNCTION__<<__LINE__;
    mCustomContextMenu->popup(QCursor::pos());
}

void HqTableWidget::initPageCtrlMenu()
{
    QMenu *menu = new QMenu(QStringLiteral("页面控制"), this);
    QList<QAction*> actlist;

    QList<struMenu> itemlist;
    itemlist.append(struMenu(QStringLiteral("首页"), FIRST_PAGE));
    itemlist.append(struMenu(QStringLiteral("前一页"), PRE_PAGE));
    itemlist.append(struMenu(QStringLiteral("后一页"), NEXT_PAGE));
    itemlist.append(struMenu(QStringLiteral("末页"), END_PAGE));

    foreach (struMenu item, itemlist) {
        QAction *act = new QAction(this);
        act->setText(item.mDisplayText);
        act->setData(item.mCmd);
        connect(act, &QAction::triggered, this, &HqTableWidget::slotSetDisplayPage);
        actlist.append(act);
    }

    menu->addActions(actlist);

    insertContextMenu(menu);
}

QAction* HqTableWidget::insertContextMenu(QMenu *menu)
{
    if(!menu) return 0;
    if(!mCustomContextMenu) mCustomContextMenu = new QMenu(this);
    return mCustomContextMenu->addMenu(menu);
}

void HqTableWidget::insertContextMenu(QAction *act)
{
    if(!act) return;
    if(!mCustomContextMenu) mCustomContextMenu = new QMenu(this);
    mCustomContextMenu->addAction(act);
}

void HqTableWidget::slotSetDisplayPage()
{
    QAction *act = (QAction*) sender();
    if(!act) return;

    int val = act->data().toInt();
    emit signalDisplayPage(val);

}

void HqTableWidget::slotCellDoubleClicked(int row, int col)
{
    return;
}

void HqTableWidget::slotCellClicked(int row, int col)
{
    this->horizontalHeader()->setHighlightSections(false);
}

void HqTableWidget::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);
    QSize size = event->size();
    qDebug()<<__func__<<__LINE__<<size;
    mMaxDisplayRow = (size.height() + mRowHeight - 1) / mRowHeight;
    mMaxDisplayCol = size.width() / mColWidth;
    mDisplayRowStart = 0;
    mDisplayRowEnd = mMaxDisplayRow-1;
    qDebug()<<"maxrow:"<<mMaxDisplayRow<<mDisplayRowStart<<mDisplayRowEnd;

    for(int i=0; i<this->columnCount(); i++)
    {
        this->setColumnWidth(i, mColWidth);
    }

    for(int i=0; i<this->columnCount(); i++)
    {
        if(i <mMaxDisplayCol)
        {
            this->setColumnHidden(i, false);
        } else
        {
            this->setColumnHidden(i, true);
        }
    }

}

#ifndef HQ_NO_GESTURE
bool HqTableWidget::gestureEvent(QGestureEvent *event)
{
    QGesture* obj = 0;
    if(obj = event->gesture(Qt::TapGesture))
    {
        //处理点击功能
        QTapGesture * gesture = static_cast<QTapGesture*>(obj);
        qDebug()<<"tap:"<<gesture<<gesture->position();

    } else if(obj = event->gesture(Qt::TapAndHoldGesture))
    {
        //QMessageBox::information(0, "hold on press", " ok");

    } else if(obj = event->gesture(Qt::PanGesture))
    {
        QPanGesture* gesture = static_cast<QPanGesture*>(obj);
        QPointF delta = gesture->delta();


    } else if(obj = event->gesture(Qt::SwipeGesture))
    {
        QSwipeGesture* gesture = static_cast<QSwipeGesture*>(obj);
        qDebug()<<gesture->horizontalDirection()<<gesture->verticalDirection();
        double angle = gesture->swipeAngle();
        QMessageBox::information(0, "Swip Gesture", QString::number(angle, 'f', 2));
        //optMoveTable(OPT_RIGHT);
    }

    return true;


}

bool HqTableWidget::event(QEvent *event)
{
    qDebug()<<"event:"<<event->type();
    if(event->type() == QEvent::Gesture)
    {
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QTableWidget::event(event);
}

#else

void HqTableWidget::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<__func__<<event;
    mPressPnt = QCursor::pos();
    mMovePnt = mPressPnt;
    mMoveDir = -1;
    QTableWidget::mousePressEvent(event);
}

void HqTableWidget::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<__func__<<event;

    //窗口跟着鼠标移动
    QPoint move_pnt = QCursor::pos();
    //判断鼠标当前是水平移动还是数值运动
    mMoveDir = 1; //0水平，1竖直
    double rad = qAbs(atan2(move_pnt.y() - mPressPnt.y(), move_pnt.x() - mPressPnt.x()));
    if( (0<=rad && rad<=0.25*3.1415926) || (rad >= 0.75 *3.1415926 && rad<=3.1415926))
    {
        mMoveDir = 0;
    }
    int move_distance = (mMoveDir == 1? move_pnt.y() - mMovePnt.y() : move_pnt.x() - mMovePnt.x());
//    qDebug()<<__func__<<mMoveDir<<move_distance;
    if(move_distance == 0) return;
#if 0
    mCurScrollBar = mMoveDir == 1? this->verticalScrollBar() : this->horizontalScrollBar();
    int scroll_max = mCurScrollBar->maximum();
    int scroll_min = mCurScrollBar->minimum();
    int endValue = int(mCurScrollBar->value() - (move_distance * 1.0 / (scroll_max - scroll_min)));
    qDebug()<<__func__<<mCurScrollBar->value()<<scroll_min<<scroll_max<<move_distance<<endValue;
    if(scroll_min > endValue)
    {
        endValue = scroll_min;
    }
    if(endValue > scroll_max)
    {
        endValue = scroll_max;
    }
    mCurScrollBar->setValue(endValue);
#else
    if(mMoveDir == 0)
    {
        if(qAbs(move_distance) < 0.5 *mColWidth) return;
        OPT_MOVE_MODE mode = move_distance < 0 ? OPT_LEFT : OPT_RIGHT;
        optMoveTable(mode);

    } else
    {
        if(qAbs(move_distance) < 0.5*mRowHeight) return;
        OPT_MOVE_MODE mode = move_distance < 0 ? OPT_UP : OPT_DOWN;
        optMoveTable(mode);
    }
#endif
    mMovePnt = move_pnt;
    //return QTableWidget::mouseMoveEvent(event);
}

void HqTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    return QTableWidget::mouseReleaseEvent(event);
}
#endif


void HqTableWidget::optMoveTable(OPT_MOVE_MODE mode)
{
    if(OPT_LEFT == mode)
    {
        //显示右边的列
        int col_start = 0, col_end = 0;
        for(int i=1; i<this->columnCount(); i++)
        {
            if(!this->isColumnHidden(i))
            {
                if(col_start == 0)
                {
                    col_start = i;
                }
            } else
            {
                if(col_start == 0) continue;
                if(col_end == 0)
                {
                    col_end = i-1;
                    break;
                }
            }
        }
        //设定新的显示的列
        //qDebug()<<"start:"<<col_start<<" end:"<<col_end;
        if(col_end == 0)
        {
            //已经到了最后，不操作
        } else
        {
            //还存在未显示的咧
            this->setColumnHidden(col_start, true);
            this->setColumnHidden(col_end + 1, false);
            updateColumn(col_end+1);
        }
        //displayVisibleCols();

    } else if(OPT_RIGHT == mode)
    {
        //显示左边的列
        int col_start = 0, col_end = 0;
        for(int i=this->columnCount()-1; i>=1; i--)
        {
            if(!this->isColumnHidden(i))
            {
                if(col_end == 0)
                {
                    col_end = i;
                }
            } else
            {
                if(col_end == 0) continue;
                if(col_start == 0)
                {
                    col_start = i+1;
                    break;
                }
            }
        }
        //qDebug()<<"start:"<<col_start<<" end:"<<col_end;
        //设定新的显示的列
        if(col_start == 0)
        {
            //已经到了最左，不操作
        } else
        {
            //还存在未显示的咧
            this->setColumnHidden(col_end, true);
            this->setColumnHidden(col_start - 1, false);
            updateColumn(col_start-1);
        }
        //displayVisibleCols();

    } else if(mode == OPT_UP)
    {
        //向上滑动，显示下面的列
        if(mDisplayRowEnd == rowCount()-1)
        {
            //next page
            emit signalDisplayPage(NEXT_PAGE);
            resetDisplayRows();
        } else
        {
            mDisplayRowStart++;
            mDisplayRowEnd++;
        }
        displayVisibleRows();
    } else if(mode == OPT_DOWN)
    {
        //向下滑动，显示上面的行
        if(mDisplayRowStart == 0)
        {
            emit signalDisplayPage(PRE_PAGE);
            resetDisplayRows();
        } else {
            mDisplayRowStart--;
            mDisplayRowEnd--;
        }
        displayVisibleRows();

    }
}

void HqTableWidget::displayVisibleCols()
{
    for(int i=0; i<columnCount(); i++)
    {
        if(isColumnHidden(i)) continue;
        //qDebug()<<"col:"<<i<<this->horizontalHeaderItem(i)->text().toLocal8Bit();
    }
}

void HqTableWidget::displayVisibleRows()
{
    //qDebug()<<"row display:"<<mDisplayRowStart<<mDisplayRowEnd;
    for(int i=0; i<rowCount(); i++)
    {
        if(i<mDisplayRowStart || i>mDisplayRowEnd) setRowHidden(i, true);
        else setRowHidden(i, false);
    }
}





