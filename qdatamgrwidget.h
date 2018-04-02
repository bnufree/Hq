#ifndef QDATAMGRWIDGET_H
#define QDATAMGRWIDGET_H

#include <QWidget>
#include "utils/sharedata.h"

namespace Ui {
class QDataMgrWidget;
}

enum DATA_MGR_TYPE{
    DATA_LHB = 0,
    DATA_MUTUAL_MARKET,
};

class QDataMgrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QDataMgrWidget(QWidget *parent = 0);
    ~QDataMgrWidget();
    void updateData();
public slots:
    void   setDislayDataType(int index);
    void   slotUpdateShareHsgtTop10List(const ShareBaseDataList& list);
    void   slotDayChanged();

private:
    Ui::QDataMgrWidget *ui;
};

#endif // QDATAMGRWIDGET_H
