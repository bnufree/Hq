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
#include "data_structure/hqutils.h"
#include <QScroller>
#include <QTimer>

#define     COL_TYPE_ROLE               Qt::UserRole + 1
#define     COL_SORT_ROLE               Qt::UserRole + 2
#define     PAGE_SIZE                   50

HqTableWidget::HqTableWidget(QWidget *parent) : QTableWidget(parent),\
    mCurScrollBar(0),
    mMoveDir(-1),
    mCustomContextMenu(0),
    mDisplayRowStart(0),
    mDisplayRowEnd(0),
    mIsWorkInMini(false),
    mLastWheelTime(0)

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
#ifdef Q_OS_WIN
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
#endif
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slotCellDoubleClicked(int,int)));
    connect(this, SIGNAL(cellClicked(int,int)),
            this, SLOT(slotCellClicked(int,int)));
    this->horizontalHeader()->setHighlightSections(false);
    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));
    //this->setAttribute(Qt::WA_AcceptTouchEvents);
#ifdef Q_OS_ANDROID
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#endif
    this->horizontalScrollBar()->setPageStep(1);
    this->verticalScrollBar()->setPageStep(1);
//    grabGesture(Qt::TapGesture);
//    grabGesture(Qt::SwipeGesture);
//    grabGesture(Qt::PanGesture);
//    grabGesture(Qt::TapAndHoldGesture);
    mRowHeight = HqUtils::convertMM2Pixel(6);
    QRect rect = QApplication::desktop()->availableGeometry();
    //设定表格的字体
    QFont font = this->font();
    font.setBold(false);
    HqUtils::setFontPixelSize(&font, mRowHeight * 0.5);
    this->setFont(font);
    //设定表格每列的名称宽度和字体
    mColWidth = HqUtils::convertMM2Pixel(14);
    HqUtils::setFontPixelSize(&font, mRowHeight * 0.5);
    this->horizontalHeader()->setFont(font);
    this->horizontalHeader()->setMinimumHeight(QFontMetrics(font).height() / 0.5);
    this->horizontalHeader()->setMaximumHeight(QFontMetrics(font).height() / 0.5);
    //设定item的大小
    //this->setStyleSheet(QString("QTableview::item{height:%1;font-weight:bold;font-size:20pt;}").arg(mRowHeight));
    this->horizontalHeader()->setBackgroundRole(this->backgroundRole());
//    QScroller::grabGesture(this, QScroller::LeftMouseButtonGesture);
#if 0
    QScroller::grabGesture(this, QScroller::TouchGesture);
#endif
#ifndef Q_QS_WIN
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
#else
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
#endif

}

bool HqTableWidget::gestureEvent(QGestureEvent *event)
{
    qDebug()<<"current gesture type:"<<event->type();
    if(event->type() == Qt::TapAndHoldGesture)
    {
        qDebug()<<event->widget();
        event->accept();
    } else
    {
        event->ignore();
    }

    return true;
}

void HqTableWidget::wheelEvent(QWheelEvent *e)
{
    if(QDateTime::currentMSecsSinceEpoch() - mLastWheelTime < 100) return;
    if(e->delta() > 0)
    {
        optMoveTable(OPT_UP, 2);
    } else
    {
        optMoveTable(OPT_DOWN, 2);
    }
    mLastWheelTime = QDateTime::currentMSecsSinceEpoch();
}

void HqTableWidget::setHeaders(const TableColDataList &list)
{
    mColWidthList.clear();
    QMenu *menu = new QMenu(QStringLiteral("列表标题"), this);
    mColDataList = list;
    this->setColumnCount(list.length());
    for(int i=0; i<mColDataList.size(); i++) {
        this->setColumnWidth(i, mColWidth);
        mColDataList[i].mColNum = i;
        this->setHorizontalHeaderItem(i, new QStkTableWidgetItem(mColDataList[i].mColStr));
        this->horizontalHeaderItem(i)->setData(COL_TYPE_ROLE, mColDataList[i].mType);
        this->horizontalHeaderItem(i)->setData(COL_SORT_ROLE, QVariant::fromValue((void*) &(mColDataList[i].mRule)));
        this->horizontalHeaderItem(i)->setFont(horizontalHeader()->font());
        //检查对应的宽度
        int wkWidth = QFontMetrics(horizontalHeader()->font()).width(mColDataList[i].mColStr);
        int realwidth = ceil(wkWidth / 0.8);
        if(realwidth >= mColWidth)
        {
            this->setColumnWidth(i, realwidth);
        }
        mColWidthList.append(columnWidth(i));
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
//    this->setColumnWidth(list.size(), 1);
//    mColWidthList.append(1);

    insertContextMenu(menu);

    //重新检查各个列的显示关系
//    checkColDisplayStatus();
}

void HqTableWidget::checkColDisplayStatus()
{
    if(mColWidthList.size() == 0) return;
    int total_display_width = width();
    int used_width = mColWidthList[0];
    bool isColStart = false;
    for(int i=1; i<columnCount(); i++)
    {
        if((!isColStart) && isColumnHidden(i)) continue;
        if(!isColumnHidden(i))
        {
            if(!isColStart) isColStart = true;
        }
        int col_width = mColWidthList[i];
        if(used_width + col_width <= total_display_width)
        {
            setColumnHidden(i, false);
            used_width += col_width;
        } else if(used_width < total_display_width)
        {
            //部分显示的情况
                setColumnHidden(i, false);
            used_width += col_width;
        } else
        {
            setColumnHidden(i, true);
        }
    }

    for(int i=0; i<columnCount(); i++)
    {
        QString colText = "";
        QTableWidgetItem* item = this->horizontalHeaderItem(i);
        if(item) colText = item->text();
        qDebug()<<i<<colText<<"hide:"<<isColumnHidden(i);
    }



}

void HqTableWidget::slotHeaderClicked(int col)
{
    int type = this->horizontalHeaderItem(col)->data(COL_TYPE_ROLE).toInt();
    emit signalSetSortType(type);
    setSortType(type);
    resetPageDisplay();
}

void HqTableWidget::slotSetColDisplay(bool isDisplay)
{
    Q_UNUSED(isDisplay)
    QAction *act = (QAction*) sender();
    if(!act) return;
    TableColData *data = (TableColData*)(act->data().value<void*>());
    if(!data) return;
    this->setColumnHidden(data->mColNum, !(data->mIsDisplay));
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
    if(font.pixelSize() > 1 && !this->isColumnHidden(column))
    {
        //字体自适应
        int text_width = QFontMetrics(font).width(text);
        while (text_width > this->columnWidth(column) - 10 && font.pixelSize() > 1) {
//            qDebug()<<"text_width:"<<text_width<<" col_width:"<<this->columnWidth(column)<<"pxel size:"<<font.pixelSize();
            HqUtils::setFontPixelSize(&font, font.pixelSize()-1);
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
    w->setFavourite(isFav);
    //QString color = isFav ? "magenta" : "white";
    //w->setStyleSheet(QString("color:%1").arg(color));
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

void HqTableWidget::keyPressEvent(QKeyEvent *event)
{
#ifdef Q_OS_WIN
    if(event->key() == Qt::Key_Left)
    {
        optMoveTable(OPT_RIGHT);
    } else if(event->key() == Qt::Key_Right)
    {
        optMoveTable(OPT_LEFT);
    } else if(event->key() == Qt::Key_Up)
    {
        optMoveTable(OPT_UP);
    } else if(event->key() == Qt::Key_Down)
    {
        optMoveTable(OPT_DOWN);
    }
#else
#endif
}

void HqTableWidget::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);

    //开始重新设定
    QTimer::singleShot(500, this, SLOT(checkDisplayStatus()));
}

void HqTableWidget::checkRowDisplayStatus()
{
    int total_display_height = this->height();
    int header_height = this->horizontalHeader()->height();
    int table_height = total_display_height - header_height;
    mMaxDisRow = table_height/ mRowHeight;
    if(mMaxDisRow * mRowHeight < table_height)
    {
        mMaxDisRow++;
    }
    //开始重新设定行的显示
    mDisplayRowEnd = mDisplayRowStart + mMaxDisRow - 1;
    updateTable();

}

void HqTableWidget::checkDisplayStatus()
{
    checkRowDisplayStatus();
    checkColDisplayStatus();
}

void HqTableWidget::mousePressEvent(QMouseEvent *event)
{
    mPressPnt = QCursor::pos();
    mMovePnt = mPressPnt;
    mMoveDir = -1;
    QTableWidget::mousePressEvent(event);
}

void HqTableWidget::mouseMoveEvent(QMouseEvent *event)
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
    int move_distance = (mMoveDir == 1? move_pnt.y() - mMovePnt.y() : move_pnt.x() - mMovePnt.x());
//    qDebug()<<__func__<<mMoveDir<<move_distance;
    if(move_distance == 0) return;
    if(mMoveDir == 0)
    {
        if(qAbs(move_distance) < 0.5 *mColWidth) return;
        OPT_MOVE_MODE mode = move_distance < 0 ? OPT_LEFT : OPT_RIGHT;
        optMoveTable(mode);

    } else
    {
        if(qAbs(move_distance) < 0.5*mRowHeight) return;
        OPT_MOVE_MODE mode = move_distance < 0 ? OPT_DOWN : OPT_UP;
        optMoveTable(mode);
    }
    mMovePnt = move_pnt;
    QTableWidget::mouseMoveEvent(event);
}

void HqTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    return QTableWidget::mouseReleaseEvent(event);
}

int  HqTableWidget::adjusVal(int val, int step, int max, int min)
{
    qDebug()<<"adjust:"<<val<<step<<max<<min;
    int res = val + step;
    if(res > max) res = max;
    if(res < min) res = min;
    return res;
}

void HqTableWidget::optMoveTable(OPT_MOVE_MODE mode, int step)
{
    qDebug()<<"now move table:"<<mode;
    if(OPT_LEFT == mode)
    {
        //显示右边的列
        int col_start = 0, col_end = 0;
        for(int i=1; i<this->columnCount(); i++)
        {
            if(isColVisible(i))
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
                    col_end = /*i-1*/i;
                    break;
                }
            }
        }
        //设定新的显示的列
        QString colText = "";
        QTableWidgetItem* item = this->horizontalHeaderItem(col_end);
        if(item) colText = item->text();
        qDebug()<<"col hide:"<<col_start<<" col display:"<<col_end<<colText;
        if(col_end == 0)
        {
            //已经到了最后，不操作
        } else
        {
            //还存在未显示的咧
            this->setColumnHidden(col_start, true);
            if(col_end < this->columnCount())
            {
                this->setColumnHidden(col_end, false);
                updateColumn(col_end);
            }
        }
       checkColDisplayStatus();

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
        checkColDisplayStatus();
    } else if(mode == OPT_UP)
    {
        //向上滑动，显示下面的列
        mDisplayRowStart--;
        if(mDisplayRowStart < 0)
        {
            mDisplayRowStart = 0;
        } else
        {
            mDisplayRowEnd--;
        }
        updateTable();

    } else if(mode == OPT_DOWN)
    {
        mDisplayRowEnd++;
        if(mDisplayRowEnd == mTotalDisplayRowCount)
        {
            mDisplayRowEnd--;
        } else
        {
            mDisplayRowStart++;
        }
        updateTable();

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


void HqTableWidget::setWorkInMini(bool sts)
{
    mIsWorkInMini = sts;
}





