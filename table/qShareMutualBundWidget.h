#ifndef QSHAREMUTUALBUNDWIDGET_H
#define QSHAREMUTUALBUNDWIDGET_H

#include "HqTableWidget.h"
//#include "dbservices/sharedata.h"
#include "data_structure/sharedata.h"
class QShareMutualBundwidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QShareMutualBundwidget(QWidget *parent = 0);
private:
signals:
    void    signalDisplayMutualBundHistory(const QString& code);

public slots:
    void    setDataList(const ShareDataList& list);
private:

};

#endif // QSHAREMUTUALBUNDWIDGET_H
