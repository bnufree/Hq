#ifndef QHQSYSTEMINFOWIDGET_H
#define QHQSYSTEMINFOWIDGET_H

#include <QWidget>

namespace Ui {
class QHqSystemInfoWidget;
}

class QShareActiveDateUpdateThread;

class QHqSystemInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QHqSystemInfoWidget(QWidget *parent = 0);
    ~QHqSystemInfoWidget();
public slots:
    void slotUpdateSystemStatus(qint64 time, int status);
private:
    Ui::QHqSystemInfoWidget *ui;
};

#endif // QHQSYSTEMINFOWIDGET_H
