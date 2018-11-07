#ifndef QDATAMGRWIDGET_H
#define QDATAMGRWIDGET_H

#include <QWidget>
#include "data_structure/sharedata.h"

namespace Ui {
class QDataMgrWidget;
}

enum DATA_MGR_TYPE{
    DATA_NONE = -1,
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
    void setDataType(int type);
public slots:
    void   setDislayDataType(int index);
    void   slotUpdateShareHsgtTop10List(const ShareDataList& list, const QString& date);
    void   slotDayChanged();

private:
    Ui::QDataMgrWidget *ui;
    int     mDataType;
};

#endif // QDATAMGRWIDGET_H
