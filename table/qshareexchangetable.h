#ifndef QSHAREEXCHANGETABLE_H
#define QSHAREEXCHANGETABLE_H

#include "HqTableWidget.h"
#include "data_structure/sharedata.h"

class QShareExchangeTablewidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QShareExchangeTablewidget(QWidget *parent = 0);
    virtual void displayNextPage();
    virtual void displayPreviousPage();
    virtual void displayFirstPage();
    virtual void displayLastPage();
public slots:
    void    setSearchCondition(const QString& code, const QString& start, const QString& end);
private:

public slots:
    void    slotRecvDataList(int page, int total_page, const QList<ShareExchangeData>& list);
    void    slotCustomContextMenuRequested(const QPoint &pos);
    void    slotCellDoubleClicked(int row, int col);
    void    slotCellClicked(int row, int col);
private:
    QString     mCode;
    QString     mStartDate;
    QString     mEndDate;
};

#endif // QSHAREEXCHANGETABLE_H
