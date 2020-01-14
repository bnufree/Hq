#ifndef QKUAIXUNLISTWIDGET_H
#define QKUAIXUNLISTWIDGET_H

#include <QWidget>
#include "data_structure/hqutils.h"

namespace Ui {
class QKuaixunListWidget;
}

class QKuaixunListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QKuaixunListWidget(QWidget *parent = 0);
    ~QKuaixunListWidget();
    void appendData(const KuaiXunList& list);

private:
    Ui::QKuaixunListWidget *ui;
    QString mLastDataID;
};

#endif // QKUAIXUNLISTWIDGET_H
