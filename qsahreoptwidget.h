#ifndef QSAHREOPTWIDGET_H
#define QSAHREOPTWIDGET_H

#include <QWidget>

namespace Ui {
class QSahreOptWidget;
}

class QSahreOptWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QSahreOptWidget(const QString& Code, QWidget *parent = 0);
    ~QSahreOptWidget();

public slots:

private slots:
    void on_add_clicked();

    void on_del_clicked();

    void on_up_clicked();

    void on_down_clicked();

private:
    Ui::QSahreOptWidget *ui;
private:
    QString             mCode;
};

#endif // QSAHREOPTWIDGET_H
