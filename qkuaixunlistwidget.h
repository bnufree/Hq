#ifndef QKUAIXUNLISTWIDGET_H
#define QKUAIXUNLISTWIDGET_H

#include <QWidget>

namespace Ui {
class QKuaixunListWidget;
}

class QKuaixunListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QKuaixunListWidget(QWidget *parent = 0);
    ~QKuaixunListWidget();

private:
    Ui::QKuaixunListWidget *ui;
};

#endif // QKUAIXUNLISTWIDGET_H
