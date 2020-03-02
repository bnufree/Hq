#ifndef QPOPWINDOW_H
#define QPOPWINDOW_H

#include <QMainWindow>

namespace Ui {
class QPopWindow;
}

class QPopWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QPopWindow(QWidget *parent = 0);
    ~QPopWindow();
public slots:
    void calc();
    void appendText(const QStringList& list);

private:
    Ui::QPopWindow *ui;
};

#endif // QPOPWINDOW_H
