#ifndef QNORTHBOUNDDISPLAYWEIDGET_H
#define QNORTHBOUNDDISPLAYWEIDGET_H

#include <QWidget>

namespace Ui {
class QNorthBoundDisPlayWeidget;
}

class QNorthBoundDisPlayWeidget : public QWidget
{
    Q_OBJECT

public:
    explicit QNorthBoundDisPlayWeidget(QWidget *parent = 0);
    ~QNorthBoundDisPlayWeidget();

private:
    Ui::QNorthBoundDisPlayWeidget *ui;
};

#endif // QNORTHBOUNDDISPLAYWEIDGET_H
