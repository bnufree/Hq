#ifndef NORTHBOUNDTOP10DISPLAYWIDGET_H
#define NORTHBOUNDTOP10DISPLAYWIDGET_H

#include "HqTableWidget.h"
#include "data_structure/sharedata.h"

class NorthBoundTop10DisplayWidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit NorthBoundTop10DisplayWidget(QWidget *parent = nullptr);

signals:

public slots:
    void slotSetDataList(const ShareHsgtList &list);
    void slotCellDoubleClicked(int row, int col);
};

#endif // NORTHBOUNDTOP10DISPLAYWIDGET_H
