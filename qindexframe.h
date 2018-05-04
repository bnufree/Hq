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
    void updateVal(double cur, double chg, double chgper, double money);
    void updateBound(double buy, double sell, double pure, double total);
    void updateBound(double shVal, QString shName, double szVal, QString szName);
    void updateBound(double pure, const QString& name);
    void setName(const QString& name);

private:
    Ui::QIndexFrame *ui;
};

#endif // QINDEXFRAME_H
