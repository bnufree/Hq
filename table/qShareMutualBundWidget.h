#ifndef QSHAREMUTUALBUNDWIDGET_H
#define QSHAREMUTUALBUNDWIDGET_H

#include "HqTableWidget.h"
//#include "dbservices/sharedata.h"
#include "utils/sharedata.h"
class QShareMutualBundwidget : public HqTableWidget
{
    Q_OBJECT
public:
    explicit QShareMutualBundwidget(QWidget *parent = 0);
private:
signals:
    void    signalDisplayMutualBundHistory(const QString& code);

public slots:
    void    setDataList(const ShareBaseDataList& list);
private:

};

#endif // QSHAREMUTUALBUNDWIDGET_H
