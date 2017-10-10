#include "HqTableWidget.h"
#include "utils/comdatadefines.h"
#include <QHeaderView>
#include <QAction>
#include "qstktablewidgetitem.h"

#define     COL_TYPE_ROLE               Qt::UserRole + 1
#define     COL_SORT_ROLE               Qt::UserRole + 2

HqTableWidget::HqTableWidget(QWidget *parent) : QTableWidget(parent),mHeaderMenu(0)
{
    mColDataList.clear();
    mColWidth = 60;
    this->verticalHeader()->setVisible(false);
    this->horizontalHeader()->setDefaultSectionSize(mColWidth);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //鼠标右键选择
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomContextMenuRequested(QPoint)));
}

void HqTableWidget::setHeaders(const TableColDataList &list)
{
    if(mHeaderMenu == NULL) mHeaderMenu = new QMenu(QStringLiteral("列表标题"), this);
    if(mHeaderMenu) mHeaderMenu->clear();
    mColDataList = list;
    this->setColumnCount(list.length());
    for(int i=0; i<mColDataList.size(); i++) {
        mColDataList[i].mColNum = i;
        this->setHorizontalHeaderItem(i, QStkTableWidgetItem(mColDataList[i].mColStr));
        this->horizontalHeaderItem(i)->setData(COL_TYPE_ROLE, mColDataList[i].mType);
        this->horizontalHeaderItem(i)->setData(COL_SORT_ROLE, QVariant::fromValue((void*) &(mColDataList[i].mRule)));
        QAction *act = new QAction(this);
        act->setText(this->horizontalHeaderItem(i)->text());
        act->setData(QVariant::fromValue((void*) &(mColDataList[i])));
        act->setCheckable(true);
        act->setChecked(mColDataList[i].mIsDisplay);
        connect(act, SIGNAL(triggered(bool)), this, SLOT(slotSetColDisplay(bool)));
        mHeaderMenu->addAction(act);
    }
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

void HqTableWidget::setFavShareList(const QString &list)
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

void HqTableWidget::updateFavShareIconOfRow(int row)
{
    if(row >= this->rowCount()) return;
    QString code = this->item(row, 0)->text().trimmed();
    if(code.left(1) == "5" || code.left(1) == "6")
    {
        code = "sh"+code;
    } else
    {
        code = "sz"+code;
    }
    if(mFavShareList.contains(code)) this->item(i, 0)->setIcon(QIcon(":/icon/image/zxg.ico"));
}

void HqTableWidget::prepareUpdateTable()
{
    mOldRowCount = this->rowCount();
}

void HqTableWidget::afterUpdateTable(int newRowCount)
{
    if(newRowCount < mOldRowCount)
    {
        for(int i=newRowCount; i<mOldRowCount; i++)
        {
            for(int k=0; k<this->columnCount(); k++)
            {
                QStkTableWidgetItem *item = (QStkTableWidgetItem*)(this->item(i, k));
                if(item)
                {
                    delete item;
                    item = 0;
                }
            }
        }
        while (mOldRowCount > newRowCount) {
            this->removeRow(mOldRowCount-1);
            mOldRowCount--;
        }
    }
}

void HqTableWidget::slotCustomContextMenuRequested(const QPoint &point)
{

}



