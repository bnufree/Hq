#ifndef QSHAREFOREIGNVOLTABLEWIDGET_H
#define QSHAREFOREIGNVOLTABLEWIDGET_H

#include "HqTableWidget.h"
#include "data_structure/sharedata.h"
#include <QThreadPool>

class QShareForeignVolTableWidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QShareForeignVolTableWidget(QWidget *parent = 0);

signals:

public slots:
    void slotFetchForeignData(const ShareForignVolFileDataList& list, const QDate& date);
    void slotFetchBtnClicked();
    void slotStartInit();

private:
    QThreadPool         mPool;
};

#endif // QSHAREFOREIGNVOLTABLEWIDGET_H
