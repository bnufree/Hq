#ifndef QSHARECODENAMELABLE_H
#define QSHARECODENAMELABLE_H

#include <QWidget>

namespace Ui {
class QShareCodeNameLable;
}

class QShareCodeNameLable : public QWidget
{
    Q_OBJECT

public:
    explicit QShareCodeNameLable(QWidget *parent = 0);
    ~QShareCodeNameLable();

private:
    Ui::QShareCodeNameLable *ui;
};

#endif // QSHARECODENAMELABLE_H
