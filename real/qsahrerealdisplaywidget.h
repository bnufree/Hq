#ifndef QSAHREREALDISPLAYWIDGET_H
#define QSAHREREALDISPLAYWIDGET_H

#include <QWidget>

namespace Ui {
class QSahreRealDisplayWidget;
}

class QSahreRealDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QSahreRealDisplayWidget(QWidget *parent = 0);
    ~QSahreRealDisplayWidget();

private:
    Ui::QSahreRealDisplayWidget *ui;
};

#endif // QSAHREREALDISPLAYWIDGET_H
