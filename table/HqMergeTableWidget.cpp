#include "HqMergeTableWidget.h"
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

HqSingleTableWidget::HqSingleTableWidget(QWidget *parent) :
    QTableWidget(parent),
    mMoveDir(-1),
    mDisplayRowStart(0),
    mDisplayRowEnd(0),
    mLastWheelTime(0)
{
    this->setItemDelegate(new RowDelegate);
    mColDataList.clear();
    this->verticalHeader()->setVisible(false);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slotCellDoubleClicked(int,int)));
    connect(this, SIGNAL(cellClicked(int,int)), this, SLOT(slotCellClicked(int,int)));
    this->horizontalHeader()->setHighlightSections(false);
    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(slotHeaderClicked(int)));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mRowHeight = HqUtils::convertMM2Pixel(6);    //设定表格的字体
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
    this->horizontalHeader()->setBackgroundRole(this->backgroundRole());
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

void HqSingleTableWidget::setHeaders(const TableColDataList &list)
{
    mColWidthList.clear();
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
        if(!mColDataList[i].mIsDisplay)
        {
            this->setColumnHidden(i, true);
        }
    }
}

void HqSingleTableWidget::slotHeaderClicked(int col)
{
    int type = this->horizontalHeaderItem(col)->data(COL_TYPE_ROLE).toInt();
    emit signalSetSortType(type);
    setSortType(type);
    resetPageDisplay();
}

void HqSingleTableWidget::appendRow()
{
    this->insertRow(this->rowCount());
}

void HqSingleTableWidget::setItemText(int row, int column, const QString &text, const QColor& color, Qt::AlignmentFlag flg)
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
            HqUtils::setFontPixelSize(&font, font.pixelSize()-1);
            text_width = QFontMetrics(font).width(text);
        }
    }
    item->setFont(font);
}

void HqSingleTableWidget::updateColumn(int col)
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



void HqSingleTableWidget::setCodeName(int row, int column, const QString &code, const QString &name)
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

void HqSingleTableWidget::updateFavShareIconOfRow(int row, bool isFav)
{
    if(row >= this->rowCount()) return;
    QShareCodeNameWidget * w = static_cast<QShareCodeNameWidget*>(cellWidget(row, 0));
    if(!w) return;
    w->setFavourite(isFav);
    //QString color = isFav ? "magenta" : "white";
    //w->setStyleSheet(QString("color:%1").arg(color));
}

void HqSingleTableWidget::prepareUpdateTable(int newRowCount)
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

void HqSingleTableWidget::removeRows(int start, int count)
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


void HqSingleTableWidget::slotCellDoubleClicked(int row, int col)
{
    return;
}

void HqSingleTableWidget::slotCellClicked(int row, int col)
{
    this->horizontalHeader()->setHighlightSections(false);
}

void HqSingleTableWidget::keyPressEvent(QKeyEvent *event)
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
    event->ignore();
#endif
}

void HqSingleTableWidget::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);

    //开始重新设定
    QTimer::singleShot(500, this, SLOT(checkDisplayStatus()));
}

void HqSingleTableWidget::checkRowDisplayStatus()
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

void HqSingleTableWidget::checkDisplayStatus()
{
    checkRowDisplayStatus();
//    checkColDisplayStatus();
}

void HqSingleTableWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    return;
    mPressPnt = QCursor::pos();
    mMovePnt = mPressPnt;
    mMoveDir = -1;
    QTableWidget::mousePressEvent(event);
}

void HqSingleTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    return;
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

void HqSingleTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
    return;
    return QTableWidget::mouseReleaseEvent(event);
}

int  HqSingleTableWidget::adjusVal(int val, int step, int max, int min)
{
    qDebug()<<"adjust:"<<val<<step<<max<<min;
    int res = val + step;
    if(res > max) res = max;
    if(res < min) res = min;
    return res;
}

void HqSingleTableWidget::optMoveTable(OPT_MOVE_MODE mode, int step)
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
//       checkColDisplayStatus();

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
//        checkColDisplayStatus();
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


int HqSingleTableWidget::getTotalColWidth() const
{
    int sum = 0;
    foreach (int i, mColWidthList) {
        sum += i;
    }

    return sum;
}


HqMergeTableWidget::HqMergeTableWidget(QWidget *parent) : QWidget(parent)
{
    mFixTable = new HqSingleTableWidget(this);
    mMoveTable = new HqSingleTableWidget(this);
    mMoveTable->setWindowFlags(mMoveTable->windowFlags() | Qt::WindowStaysOnBottomHint);
    mMovColCount = 0;
    //设定抬头
    TableColDataList datalist;
    datalist.append(TableColData(QStringLiteral("名称"), STK_DISPLAY_SORT_TYPE_NAME));
    datalist.append(TableColData(QStringLiteral("最新"), STK_DISPLAY_SORT_TYPE_PRICE));
    datalist.append(TableColData(QStringLiteral("涨跌(%)"), STK_DISPLAY_SORT_TYPE_CHGPER));
    datalist.append(TableColData(QStringLiteral("成交(亿)"), STK_DISPLAY_SORT_TYPE_CJE));
    datalist.append(TableColData(QStringLiteral("资金比"), STK_DISPLAY_SORT_TYPE_MONEYR));
    datalist.append(TableColData(QStringLiteral("周变动"), STK_DISPLAY_SORT_TYPE_LAST_WEEK));
    datalist.append(TableColData(QStringLiteral("月变动"), STK_DISPLAY_SORT_TYPE_LAST_MONTH));
    datalist.append(TableColData(QStringLiteral("年变动"), STK_DISPLAY_SORT_TYPE_LAST_YEAR,true));
    datalist.append(TableColData(QStringLiteral("资金(万)"), STK_DISPLAY_SORT_TYPE_ZJLX));
//    datalist.append(TableColData(QStringLiteral("股息率%"), STK_DISPLAY_SORT_TYPE_GXL, true));
//    datalist.append(TableColData(QStringLiteral("送转"), STK_DISPLAY_SORT_TYPE_SZZBL, true));
    datalist.append(TableColData(QStringLiteral("市值(亿)"), STK_DISPLAY_SORT_TYPE_TCAP));
    datalist.append(TableColData(QStringLiteral("流通市值(亿)"), STK_DISPLAY_SORT_TYPE_MCAP, true));
//    datalist.append(TableColData(QStringLiteral("净资产收益率(%)"), STK_DISPLAY_SORT_TYPE_JZCSYL, true));
//    datalist.append(TableColData(QStringLiteral("外资持股(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL));
//    datalist.append(TableColData(QStringLiteral("外资持股△1(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG));
//    datalist.append(TableColData(QStringLiteral("外资持股△5(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG5));
//    datalist.append(TableColData(QStringLiteral("外资持股△10(万)"), STK_DISPLAY_SORT_TYPE_FOREIGN_VOL_CHG10));
    datalist.append(TableColData(QStringLiteral("持股市值(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP));
    datalist.append(TableColData(QStringLiteral("持股市值△(亿)"), STK_DISPLAY_SORT_TYPE_FOREIGN_CAP_CHG));
    datalist.append(TableColData(QStringLiteral("换手率(%)"), STK_DISPLAY_SORT_TYPE_HSL));
    datalist.append(TableColData(QStringLiteral("登记日"), STK_DISPLAY_SORT_TYPE_GQDJR));
    datalist.append(TableColData(QStringLiteral("公告日"), STK_DISPLAY_SORT_TYPE_YAGGR));

    setHeaders(datalist, datalist.size() -1);
}

HqMergeTableWidget::~HqMergeTableWidget()
{

}

void HqMergeTableWidget::setFixTable(HqSingleTableWidget *fix)
{
    mFixTable = fix;
    resetPos();
}

void HqMergeTableWidget::setMovTable(HqSingleTableWidget *mov)
{
    mMoveTable = mov;
    resetPos();;
}

void HqMergeTableWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);
    resetPos();
}

void HqMergeTableWidget::resetPos(int move_pos, bool isok)
{
    int used_width = 0;
    //获取固定表的宽度
    used_width = mFixTable->getTotalColWidth();
    mFixTable->resize(used_width, height());
    //移动表紧挨着
    mMoveTable->resize(mMoveTable->getTotalColWidth(), height());
    if(!isok) move_pos = used_width;
    mMoveTable->move(move_pos, 0);

    mFixTable->move(0, 0);
}


void HqMergeTableWidget::mousePressEvent(QMouseEvent *event)
{
    mPressPnt = QCursor::pos();
    mMovePnt = mPressPnt;
    mMoveDir = -1;
    QWidget::mousePressEvent(event);
}

void HqMergeTableWidget::mouseMoveEvent(QMouseEvent *event)
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
    if(move_distance == 0) return;
    if(mMoveDir == 0)
    {
        if(mFixTable->getTotalColWidth() + mMoveTable->getTotalColWidth() <= width()) return;
        if(qAbs(move_distance) < 2) return;
        OPT_MOVE_MODE mode = move_distance < 0 ? OPT_LEFT : OPT_RIGHT;
        if(mode == OPT_LEFT)
        {
            if(mMoveTable->geometry().right() <= width()) return;
            int left = mMoveTable->geometry().left() - 0.5*mMoveTable->getColWidth();;
            resetPos(left, true);
//            if(mMoveTable->geometry().right() <= width())
//            {
//                left = width() - mMoveTable->geometry().width();
//            }
        } else
        {
            if(mMoveTable->geometry().left() >= mFixTable->getTotalColWidth()) return;
            int left = mMoveTable->geometry().left() + 0.5*mMoveTable->getColWidth();
//            if(left + mMoveTable->geometry().width() >= width())
//            {
//                left = width() - mMoveTable->geometry().width();
//            }
            resetPos(left, true);
        }

    } else
    {
        if(qAbs(move_distance) < 2) return;
        OPT_MOVE_MODE mode = move_distance < 0 ? OPT_DOWN : OPT_UP;
        mFixTable->optMoveTable(mode);
        mMoveTable->optMoveTable(mode);
    }
    mMovePnt = move_pnt;
    QWidget::mouseMoveEvent(event);
}

void HqMergeTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    return QWidget::mouseReleaseEvent(event);
}


void HqMergeTableWidget::keyPressEvent(QKeyEvent *event)
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
    event->ignore();
#endif
}


void HqMergeTableWidget::setHeaders(const TableColDataList &list, int move_count)
{
    mMovColCount = move_count;
    TableColDataList fix_list = list.mid(0, list.size() - move_count);
    if(fix_list.size() > 0)
    {
        if(mFixTable) mFixTable->setHeaders(fix_list);
    }
    TableColDataList mov_list = list.mid(fix_list.size());
    if(mov_list.size() > 0)
    {
        if(mMoveTable) mMoveTable->setHeaders(mov_list);
    }
}








