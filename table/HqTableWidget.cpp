#include "HqTableWidget.h"
#include "utils/comdatadefines.h"
#include <QHeaderView>
#include <QAction>
#include "qstktablewidgetitem.h"
#include <QDebug>
#include <QScrollBar>

#define     COL_TYPE_ROLE               Qt::UserRole + 1
#define     COL_SORT_ROLE               Qt::UserRole + 2

HqTableWidget::HqTableWidget(QWidget *parent) : QTableWidget(parent),\
    mCurScrollBar(0),
    mMoveDir(-1),
    mCustomContextMenu(0)
{
    initPageCtrlMenu();
    mColDataList.clear();
//    mColWidth = 60;
    this->verticalHeader()->setVisible(false);
//    this->horizontalHeader()->setDefaultSectionSize(mColWidth);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
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
    emit signalSetSortType(this->horizontalHeaderItem(col)->data(COL_TYPE_ROLE).toInt());/*
    int *rule = (int*)(this->horizontalHeaderItem(col)->data(COL_SORT_ROLE).value<void*>());
    *rule = !(rule);
    emit signalSetSortRule(*rule);*/
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

void HqTableWidget::setItemText(int row, int column, const QString &text, Qt::AlignmentFlag flg)
{
    QStkTableWidgetItem *item = (QStkTableWidgetItem*) (this->item(row, column));
    if(item)
    {
        item->setString(text);
    }
    else
    {
        this->setItem(row, column, new QStkTableWidgetItem(text, flg));
    }
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
    if(isFav)
    {
        this->item(row, 0)->setIcon(QIcon(":/icon/image/zxg.ico"));
    } else
    {
        this->item(row, 0)->setIcon(QIcon());
    }
}

void HqTableWidget::prepareUpdateTable(int newRowCount)
{
    int oldRowCount = this->rowCount();
    if(oldRowCount < newRowCount)
    {
        this->setRowCount(newRowCount);
    } else if(oldRowCount > newRowCount)
    {
        removeRows(newRowCount, oldRowCount - newRowCount);
    } else
    {
        //do nothing
    }
}

void HqTableWidget::removeRows(int start, int count)
{
    for(int i=0; i<count; i++)
    {
        for(int k=0; k<this->columnCount(); k++)
        {
            QStkTableWidgetItem *item = (QStkTableWidgetItem*)(this->item(start, k));
            if(item)
            {
                delete item;
                item = 0;
            }
        }
        this->removeRow(start);
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
    mMaxDisplayRow = 10;
    mMaxDisplayCol = 4;
    if(size.height() < size.width())
    {
        mMaxDisplayRow = 5;
        mMaxDisplayCol = 8;
        if(mMaxDisplayCol > this->columnCount())
        {
            mMaxDisplayCol = this->columnCount();
        }
    }
    for(int i=0; i<this->rowCount(); i++)
    {
        this->setRowHeight(i, size.height() / mMaxDisplayRow);
    }

    for(int i=0; i<this->columnCount(); i++)
    {
        this->setColumnWidth(i,size.width()/ mMaxDisplayCol);
    }
    this->horizontalScrollBar()->setMaximum(size.width()-this->horizontalScrollBar()->pageStep());
    this->verticalScrollBar()->setMaximum(size.height()-this->verticalScrollBar()->pageStep());
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


    for(int i=0; i<this->rowCount(); i++)
    {
        if(i <mMaxDisplayRow)
        {
            this->setRowHidden(i, false);
        } else
        {
            this->setRowHidden(i, true);
        }
    }



}

void HqTableWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<__func__<<event;
    mPressPnt = QCursor::pos();
    mMovePnt = mPressPnt;
    mMoveDir = -1;
    QTableWidget::mousePressEvent(event);
}

void HqTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<__func__<<event;

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
    qDebug()<<__func__<<mMoveDir<<move_distance;
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
        if(move_distance < 0)
        {
            //从右往左，显示右边的列数
            for(int i=2; i<this->columnCount(); i++)
            {
                if(this->isColumnHidden(i)) continue;
                if(this->columnCount() - i == mMaxDisplayCol - 2) break;
                this->setColumnHidden(i, true);
                if(i+mMaxDisplayCol - 2 < this->columnCount())
                this->setColumnHidden(i+mMaxDisplayCol - 2, false);
            }


        } else if(move_distance > 0)
        {
            //从左往右，显示左边的列数
            for(int i=this->columnCount()-1; i>=2; i++)
            {
                if(!this->isColumnHidden(i)) continue;
                if(i == mMaxDisplayCol - 1) break;
                this->setColumnHidden(i, true);
                this->setColumnHidden(i-mMaxDisplayCol+ 2, false);
            }
        }
        //显示当前的显示列
        QString colstr;
        for(int i=0; i<this->columnCount(); i++)
        {
            if(!this->isColumnHidden(i))
            colstr += QString::number(i+1);
        }
        qDebug()<<__func__<<"display col:"<<colstr;
    } else
    {
        if(move_distance < 0)
        {
            //从右往左，显示右边的列数


        } else if(move_distance > 0)
        {
            //从左往右，显示左边的列数
        }
    }
#endif
    mMovePnt = move_pnt;
    //return QTableWidget::mouseMoveEvent(event);
}

void HqTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    return QTableWidget::mouseReleaseEvent(event);
}

#if 0
bool HqTableWidget::event(QEvent *event)
{
    return QTableWidget::event(event);
    //根据鼠标的动作——按下、放开、拖动，执行相应的操作
    if(event->type() == QEvent::MouseButtonPress)
    {
        //记录按下的时间、坐标
        mPressPnt = QCursor::pos();
        mMovePnt = mPressPnt;
        mMoveDir = -1;
        return true;
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        mCurScrollBar = 0;
        mMoveDir = -1;
        return true;
    }
    else if(event->type() == QEvent::MouseMove)
    {
        //窗口跟着鼠标移动
        QPoint move_pnt = QCursor::pos();
        //判断鼠标当前是水平移动还是数值运动
        mMoveDir = 1; //0水平，1竖直
        double rad = qAbs(atan2(move_pnt.y() - mPressPnt.y(), move_pnt.x() - mPressPnt.x()));
        if( (0<=rad && rad<=0.25*3.1415926) || (rad >= 0.75 *3.1415926 && rad<=3.1415926))
        {
            mMoveDir = 0;
        }
        mCurScrollBar = mMoveDir == 1? this->verticalScrollBar() : this->horizontalScrollBar();
        int move_distance = mMoveDir == 1? move_pnt.y() - mMovePnt.y() : move_pnt.x() - mMovePnt.x();
        int endValue = mCurScrollBar->value() - move_distance;
        int scroll_max = mCurScrollBar->maximum();
        int scroll_min = mCurScrollBar->minimum();
        if(scroll_min > endValue)
        {
            endValue = scroll_min;
        }
        if(endValue > scroll_max)
        {
            endValue = scroll_max;
        }
        mCurScrollBar->setValue(endValue);
        mMovePnt = move_pnt;
        return true;
    }

}

#endif



