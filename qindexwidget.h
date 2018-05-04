#ifndef QINDEXWIDGET_H
#define QINDEXWIDGET_H

#include <QStackedWidget>
//#include "dbservices/sharedata.h"
#include "qindexframe.h"
#include "utils/sharedata.h"
#include <QTimer>
#include <QResizeEvent>

class ShareDataList;
class QIndexWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit QIndexWidget(QWidget *parent = 0);
    ~QIndexWidget();

    void resizeEvent(QResizeEvent *e);
signals:

public slots:
    void updateData(const ShareDataList& list);
    void updateData(const QList<NS_BOUND_DATA> &list);
    void insetWidget(const QString& code);
    void switchWidget();

private:
    QMap<QString, QWidget*> mIndexWidgetMap;
    QTimer*             mSwitchTimer;
};

#endif // QINDEXWIDGET_H
