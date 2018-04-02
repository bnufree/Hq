#ifndef QANDROIDBUTTON_H
#define QANDROIDBUTTON_H

#include <QLabel>

class QAndroidButton : public QLabel
{
    Q_OBJECT
public:
    explicit QAndroidButton(const QString& label, QWidget *parent = 0);
    explicit QAndroidButton(QWidget *parent = 0) : QLabel(parent) {}

signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *ev);
    void focusOutEvent(QFocusEvent *ev);
public slots:
};

#endif // QANDROIDBUTTON_H
