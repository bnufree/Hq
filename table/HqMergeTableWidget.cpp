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
#include <QHBoxLayout>
#include <Qtimer>

#define     COL_TYPE_ROLE               Qt::UserRole + 1
#define     COL_SORT_ROLE               Qt::UserRole + 2

HqSingleTableWidget::HqSingleTableWidget(QWidget *parent) :
    QTableWidget(parent)
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
    int row_height = 7;
    int col_width = 14;
#ifdef Q_OS_WIN
    row_height = 10;
    col_width = 25;
#endif
    mRowHeight = HqUtils::convertMM2Pixel(row_height);    //设定表格的字体
    QFont font = this->font();
    font.setBold(false);
    HqUtils::setFontPixelSize(&font, mRowHeight * 0.5);
    this->setFont(font);
    //设定表格每列的名称宽度和字体
    mColWidth = HqUtils::convertMM2Pixel(col_width);
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

bool HqSingleTableWidget::doubleClickedCheck(const QPoint &global_pos)
{
    QPoint table_pos = this->mapFromParent(global_pos);
    QTableWidgetItem* item = itemAt(table_pos);
    if(item)
    {
        emit cellDoubleClicked(item->row() - 1, item->column());
        return true;
    }

    return false;
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
     qDebug()<<"$$$$$$$$$$$$$$$$$$$$$$$$"<<row<<col;
    return;
}

void HqSingleTableWidget::slotCellClicked(int row, int col)
{
    qDebug()<<"!!!!!!!!!!!!!!!1"<<row<<col;
    this->horizontalHeader()->setHighlightSections(false);
}


void HqSingleTableWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    return;
}

void HqSingleTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    return;
}

void HqSingleTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
    return;
}

void HqSingleTableWidget::wheelEvent(QWheelEvent *event)
{
    event->ignore();
    return;
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
    mDoubleClickTimer = NULL;
    mDisplayRowStart = 0;
    mTotalRowCount = 0;
    QHBoxLayout *lay = new QHBoxLayout(this);
    this->setLayout(lay);
    mFixTable = new HqSingleTableWidget(this);
    QFrame* move_frm = new QFrame(this);
    mMoveTable = new HqSingleTableWidget(move_frm);
    lay->addWidget(mFixTable);
    lay->addWidget(move_frm);
    lay->setSpacing(0);
    lay->setMargin(0);
    mMovColCount = 0;
    mLastMoveTime = 0;
    mRowHeight = mMoveTable->getRowHeight();

    connect(mFixTable, &HqSingleTableWidget::signalSetSortType, this, &HqMergeTableWidget::setSortType);
    connect(mMoveTable, &HqSingleTableWidget::signalSetSortType, this, &HqMergeTableWidget::setSortType);
    connect(mFixTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slotCellDoubleClicked(int,int)));
//    connect(mMoveTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slotCellDoubleClicked(int,int)));
    mUpdateTimer = new QTimer(this);
    connect(mUpdateTimer, &QTimer::timeout, this, &HqMergeTableWidget::updateTableInfo);
    mUpdateTimer->setInterval(1000);
    mUpdateTimer->start();
}

HqMergeTableWidget::~HqMergeTableWidget()
{

}



void HqMergeTableWidget::resizeEvent(QResizeEvent *e)
{
    mMoveTable->resize(mMoveTable->getTotalColWidth(), e->size().height());
    mMoveTable->move(0, 0);

    int total_display_height = e->size().height();
    int header_height = mMoveTable->horizontalHeader()->height();
    int table_height = total_display_height - header_height;
    mMaxDisRow = table_height/ mRowHeight;
    if(mMaxDisRow * mRowHeight < table_height)
    {
        mMaxDisRow++;
    }

    QWidget::resizeEvent(e);
}

void HqMergeTableWidget::mousePressEvent(QMouseEvent *e)
{
    mPressPnt = QCursor::pos();
    mMovePnt = mPressPnt;
    mMoveDir = -1;
    QWidget::mousePressEvent(e);
    if(!mDoubleClickTimer)
    {
        mDoubleClickTimer = new QTimer(this);
        mDoubleClickTimer->setInterval(1000);
        connect(mDoubleClickTimer, &QTimer::timeout, [=](){
            mDoubleClickTimer->stop();
            delete mDoubleClickTimer;
            mDoubleClickTimer = 0;
        });

        mDoubleClickTimer->start();
    } else
    {
        if(mDoubleClickTimer->isActive())
        {
            mDoubleClickTimer->stop();
            delete mDoubleClickTimer;
            mDoubleClickTimer = 0;
            if(!mFixTable->doubleClickedCheck(e->pos()))
            {
                mMoveTable->doubleClickedCheck(e->pos());
                return;
            }

        }
    }
}
void HqMergeTableWidget::moveTable(int mode)
{
    if(mode == OPT_LEFT || mode == OPT_RIGHT)
    {
        if(mFixTable->getTotalColWidth() + mMoveTable->getTotalColWidth() <= width()) return;
        int step = 0.30*mMoveTable->getColWidth();
        if(mode == OPT_LEFT)
        {
            if(mMoveTable->geometry().right() <=  mMoveTable->parentWidget()->width()) return;
            int left = mMoveTable->geometry().left() - step;
            if(left + mMoveTable->width() <= mMoveTable->parentWidget()->width())
            {
                left = mMoveTable->parentWidget()->width() - mMoveTable->width();
            }
            mMoveTable->move(left, 0);
        } else
        {
            if(mMoveTable->geometry().left() >= 0) return;
            int left = mMoveTable->geometry().left() + step;
            if(left >= 0)
            {
                left = 0;
            }
            mMoveTable->move(left, 0);
        }

    } else
    {
        if(mode == OPT_UP)
        {
            //向上滑动，显示下面的列
            mDisplayRowStart --;
            if(mDisplayRowStart < 0)
            {
                mDisplayRowStart = 0;
            }
//            updateTable();

        } else if(mode == OPT_DOWN)
        {
            mDisplayRowStart++;
            if(mDisplayRowStart > mTotalRowCount - 1)
            {
                mDisplayRowStart = mTotalRowCount - 1;
            }
//            updateTable();

        }
    }

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
    if(QDateTime::currentMSecsSinceEpoch() - mLastMoveTime < 100) return;
    OPT_MOVE_MODE mode = OPT_LEFT;
    if(mMoveDir == 0)
    {
        mode = move_distance < 0 ? OPT_LEFT : OPT_RIGHT;
    } else
    {
        mode = move_distance < 0 ? OPT_DOWN : OPT_UP;
    }
    moveTable(mode);

    mMovePnt = move_pnt;
    mLastMoveTime = QDateTime::currentMSecsSinceEpoch();
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
        moveTable(OPT_RIGHT);
    } else if(event->key() == Qt::Key_Right)
    {
        moveTable(OPT_LEFT);
    } else if(event->key() == Qt::Key_Up)
    {
        moveTable(OPT_UP);
    } else if(event->key() == Qt::Key_Down)
    {
        moveTable(OPT_DOWN);
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
        if(mFixTable)
        {
            mFixTable->setHeaders(fix_list);
            mFixTable->setFixedWidth(mFixTable->getTotalColWidth());
        }
    }
    TableColDataList mov_list = list.mid(fix_list.size());
    if(mov_list.size() > 0)
    {
        if(mMoveTable)
        {
            mMoveTable->setHeaders(mov_list);
            mMoveTable->setFixedWidth(mMoveTable->getTotalColWidth());
        }
    }
}

void HqMergeTableWidget::prepareUpdateTable(int newRowCount)
{
    mFixTable->prepareUpdateTable(newRowCount);
    mMoveTable->prepareUpdateTable(newRowCount);
}

void HqMergeTableWidget::removeRows(int start, int count)
{
    mFixTable->removeRows(start, count);
    mMoveTable->removeRows(start, count);
}

void HqMergeTableWidget::resetDisplayRows()
{
    mDisplayRowStart = 0;
}









