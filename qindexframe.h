#ifndef QINDEXFRAME_H
#define QINDEXFRAME_H

#include <QWidget>

namespace Ui {
class QIndexFrame;
}

class QIndexFrame : public QWidget
{
    Q_OBJECT

public:
    explicit QIndexFrame(const QString& name, QWidget *parent = 0);
    ~QIndexFrame();
    void updateVal(double cur, double chg, double chgper);
    void setName(const QString& name);

private:
    Ui::QIndexFrame *ui;
};

#endif // QINDEXFRAME_H
