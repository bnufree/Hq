#ifndef QINDEXWIDGET_H
#define QINDEXWIDGET_H

#include <QStackedWidget>
//#include "dbservices/sharedata.h"
#include "qindexframe.h"
#include "data_structure/sharedata.h"
#include <QTimer>
#include <QResizeEvent>

#if 0
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
    void updateData(const ShareHsgtList &list);
    void insertWidget(const QString& code);
    void switchWidget();
    void insertWidget(QWidget* w);
    void insertWidget(const QWidgetList& list);

private:
    QMap<QString, QWidget*> mIndexWidgetMap;
    QTimer*             mSwitchTimer;
};
#else
class QHqIndexThread;
class QIndexWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QIndexWidget(QWidget *parent = 0);
    ~QIndexWidget();

    void resizeEvent(QResizeEvent *e);
signals:

public slots:
    void updateData(const ShareDataList& list, qint64 time);
    void updateData(const ShareHsgtList &list);
    void insertWidget(const QString& code);
    void switchWidget();
    void updateIndexDataList(const QList<IndexFrameData>& list);
private:
    QIndexFrame* frameOfData(const IndexFrameData& data);

private:
    QTimer*                     mSwitchTimer;
    int                         mMaxDisplayFrameCount;
    QList<QIndexFrame*>         mWidgetList;
    QHqIndexThread*             mIndexThread;

};
#endif

#endif // QINDEXWIDGET_H
