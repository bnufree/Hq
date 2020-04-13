#ifndef QKUAIXUNLISTWIDGET_H
#define QKUAIXUNLISTWIDGET_H

#include <QWidget>
#include "data_structure/hqutils.h"
#include <QLabel>

class QTextBrowser;
namespace Ui {
class QKuaixunListWidget;
}

enum{
    Confort_Width = 1,
    Confort_Height = 2,
    Confort_Both = 4,
};

class ConfortableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ConfortableLabel(const QString& text, QWidget* parent = 0);
    void updateWidth(int width);
};

class QInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QInfoWidget(const KuaixunData& data, QWidget *parent = 0);
    ~QInfoWidget() {}
    void setReferWidth(int width);
private:
    ConfortableLabel *mTitle;
    QTextBrowser *mContent;
};

class QListWidgetItem;
class QKuaixunListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QKuaixunListWidget(QWidget *parent = 0);
    ~QKuaixunListWidget();
public slots:
    void appendData(const KuaiXunList& list);
    void itemDoubleClicked(QListWidgetItem* item);
signals:
    void sendRecvInfoList(const KuaiXunList& list);
protected:

private:
    Ui::QKuaixunListWidget *ui;
    QString mLastDataID;
};

#endif // QKUAIXUNLISTWIDGET_H
