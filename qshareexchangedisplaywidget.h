#ifndef QSHAREEXCHANGEDISPLAYWIDGET_H
#define QSHAREEXCHANGEDISPLAYWIDGET_H

#include <QWidget>
#include <QStackedWidget>

class QAndroidButton;

namespace Ui {
class QShareExchangeDisplayWidget;
}

class QShareExchangeDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QShareExchangeDisplayWidget(QWidget *parent = 0);
    ~QShareExchangeDisplayWidget();
private:
    void    addBtnAndTable(QLayout* layout, const QString& title, QWidget* w);

private:
    Ui::QShareExchangeDisplayWidget *ui;
    QStackedWidget*                 mContainter;
};

#endif // QSHAREEXCHANGEDISPLAYWIDGET_H
