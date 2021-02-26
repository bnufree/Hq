#ifndef ZCHXDISPLAYWIDGET_H
#define ZCHXDISPLAYWIDGET_H

#include <QWidget>

class zchxDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit zchxDisplayWidget(QWidget *parent = nullptr);

public slots:
    virtual void updateDisplay();

signals:

};

#endif // ZCHXDISPLAYWIDGET_H
