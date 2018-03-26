#ifndef QINDEXWIDGET_H
#define QINDEXWIDGET_H

#include <QWidget>
//#include "dbservices/sharedata.h"
#include "qindexframe.h"
#include "utils/sharedata.h"

class ShareDataList;
class QIndexWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QIndexWidget(QWidget *parent = 0);
    ~QIndexWidget();


signals:

public slots:
    void updateData(const ShareDataList& list);
    void updateData(const QList<NS_BOUND_DATA> &list);
    void insetWidget(const QString& code);

private:
    QMap<QString, QIndexFrame*> mIndexWidgetMap;
};

#endif // QINDEXWIDGET_H
