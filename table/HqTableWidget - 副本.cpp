#include "HqTableWidget.h"
#include "utils/comdatadefines.h"
#include <QHeaderView>
#include <QAction>
#include "qstktablewidgetitem.h"
#include <QDebug>
#include <QScrollBar>
#include <QPropertyAnimation>

#define     COL_TYPE_ROLE               Qt::UserRole + 1
#define     COL_SORT_ROLE               Qt::UserRole + 2

HqTableWidget::HqTableWidget(QWidget *parent) : QTableWidget(parent),\
    mCurScrollBar(0),
    mMoveDir(-1),
    mCustomContextMenu(0)
{
    initPageCtrlMenu();
    mColDataList.clear();
 //   QObjectList objectList = this->children();
//    for(int i = 0; i < objectList.count(); i++)
//    {
//        if(objectList.at(i)->objectName() == "qt_scrollarea_viewport")
//        {
//            objectList.at(i)->installEventFilter(this);
//        }
//    }
//    qDebug()<<objectList;

    this->verticalHeader()->setVisible(false);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //鼠标右键选择
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slotCellDoubleClicked(int,int)));
    this->horizontalHeader()->setHighlightSections(false);
    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    QList<Qt::GestureType> gestures;
//    gestures<<Qt::PinchGesture<<Qt::PanGesture<<Qt::TapGesture<<Qt::TapAndHoldGesture<<Qt::SwipeGesture;
//    foreach (Qt::GestureType type, gestures) {
//        grabGesture(type);
//    }
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
    int rowNum = 10;
    int colNum = 4;
    if(size.height() < size.width())
    {
        rowNum = 5;
        colNum = 8;
        if(colNum > this->columnCount())
        {
            colNum = this->columnCount();
        }
    }
    for(int i=0; i<this->rowCount(); i++)
    {
        this->setRowHeight(i, size.height() / rowNum);
    }

    for(int i=0; i<this->columnCount(); i++)
    {
        this->setColumnWidth(i,size.width()/ colNum);
    }


}

bool HqTableWidget::event(QEvent *event)
{
    if(event->type() == QEvent::Gesture)
    {
        return gestureEvent(static_cast<QGestureEvent*> (event));
    }

    return QTableWidget::event(event);
}

bool HqTableWidget::gestureEvent(QGestureEvent *event)
{
    qDebug()<<__func__<<event->gestures();
}


//bool HqTableWidget::eventFilter(QObject *obj, QEvent *event)
//{
//    return QTableWidget::eventFilter(obj, event);
////    static int press_y   = 0;
////    static int move_y    = -1;
////    static int release_y = 0;
////    static int press_x = 0;
////    static int move_x = -1;
////    static int release_x = 0;
//    static QDateTime pressDateTime;
////    static QPropertyAnimation *animation = new QPropertyAnimation();

//    if("qt_scrollarea_viewport" != obj->objectName())
//    {
//        return false;
//    }
//    //根据鼠标的动作——按下、放开、拖动，执行相应的操作
//    if(event->type() == QEvent::MouseButtonPress)
//    {
//        //记录按下的时间、坐标
//        mPressPnt = QCursor::pos();
//        pressDateTime = QDateTime::currentDateTime();
//        mMovePnt = mPressPnt;
//        mMoveDir = -1;
//        //press_y = move_y;
//        //animation->stop();
//    }
//    else if(event->type() == QEvent::MouseButtonRelease)
//    {
//#if 0
//        //鼠标放开，根据鼠标拖动的垂直距离和持续时间，设置窗口滚动快慢程度和距离
//        if(animation->targetObject() != mCurScrollBar)
//        {
//            animation->setTargetObject(mCurScrollBar);
//            animation->setPropertyName("value");
//        }
//        mMovePnt = QCursor::pos();
//        QObject *parent_obj = obj->parent();
//        if(parent_obj != 0 || parent_obj->inherits("QAbstractItemView"))
//        {
//            QTimer::singleShot(150, (QAbstractItemView *)parent_obj
//                               , SLOT(clearSelection()));
//        }
//        int endValue = 0;
//        int pageStep = 0;
//        int scroll_max = mCurScrollBar->maximum();
//        int scroll_min = mCurScrollBar->minimum();
//        if( (mMoveDir == 1 && qAbs(mMovePnt.y() - mPressPnt.y()) > 45) ||\
//                (mMoveDir == 0 && qAbs(mMovePnt.x() - mPressPnt.x()) > 45))
//        {
//            int mseconds = pressDateTime.msecsTo(QDateTime::currentDateTime());
//            int limit = 440;
//            pageStep = 240;//scrollBarV->pageStep();
//            //            qDebug()<<"pageStep="<<pageStep;
//            if(mseconds > limit)//滑动的时间大于某个值的时候，不再滚动(通过增加分母)
//            {
//                mseconds = mseconds + (mseconds - limit) * 20;
//            }

//            if(release_y - press_y > 0)
//            {
//                endValue = m_scrollBarV->value()
//                        - pageStep * (200.0 / mseconds);//.0避免避免强制转换为整形
//                if(scrollV_min > endValue)
//                {
//                    endValue = scrollV_min;
//                }
//            }
//            else if(release_y - press_y < 0)
//            {
//                    endValue = m_scrollBarV->value() + pageStep * (200.0 / mseconds);
//                    if(endValue > scrollV_max)
//                    {
//                        endValue = scrollV_max;
//                    }
//            }
//            if(mseconds > limit)
//            {
//                mseconds = 0;//滑动的时间大于某个值的时候，滚动距离变小，减小滑动的时间
//            }
//            animation->setDuration(mseconds+550);
//            animation->setEndValue(endValue);
//            animation->setEasingCurve(QEasingCurve::OutQuad);
//            animation->start();
//            return true;
//        }
//#endif
//        return true;
//    }
//    else if(event->type() == QEvent::MouseMove)
//    {
//        //窗口跟着鼠标移动
//        QPoint move_pnt = QCursor::pos();
//        //判断鼠标当前是水平移动还是数值运动
//        mMoveDir = 1; //0水平，1竖直
//        double rad = qAbs(atan2(move_pnt.y() - mPressPnt.y(), move_pnt.x() - mPressPnt.x()));
//        if( (0<=rad && rad<=0.25*3.1415926) || (rad >= 0.75 *3.1415926 && rad<=3.1415926))
//        {
//            mMoveDir = 0;
//        }
//        mCurScrollBar = mMoveDir == 1? this->verticalScrollBar() : this->horizontalScrollBar();
//        int move_distance = mMoveDir == 1? move_pnt.y() - mMovePnt.y() : move_pnt.x() - mMovePnt.x();
//        int endValue = mCurScrollBar->value() - move_distance;
//        int scroll_max = mCurScrollBar->maximum();
//        int scroll_min = mCurScrollBar->minimum();
//        if(scroll_min > endValue)
//        {
//            endValue = scroll_min;
//        }
//        if(endValue > scroll_max)
//        {
//            endValue = scroll_max;
//        }
//        mCurScrollBar->setValue(endValue);
//        mMovePnt = move_pnt;
//    }
//    return false;
//}




