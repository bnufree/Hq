#ifndef NORTHBOUNDMONEYFLOWTABLEWIDGET_H
#define NORTHBOUNDMONEYFLOWTABLEWIDGET_H

#include <QWidget>

class NorthBoundMoneyFlowTableWidget;
class QLabel;
class QPushButton;
class QTableWidget;

class NorthBoundMoneyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NorthBoundMoneyWidget(QWidget *parent = nullptr);
    void     appendWidget(NorthBoundMoneyFlowTableWidget* w);

signals:

public slots:
private:


};

class NorthBoundMoneyFlowTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NorthBoundMoneyFlowTableWidget(QWidget* display, QWidget *parent = nullptr);
    void    setWorkMode(bool mode);
    bool     workDetail() const {return mWorkDetail;}
    void    setChangeModeAvailable(bool sts);
    virtual  void updateData() {}

signals:

public slots:
    void    slotChangeDisplayMode();
private:
    QLabel              *mTimeLabel;
    QPushButton         *mDetailBtn;
    QWidget             *mDisplayWidget;
    bool                 mWorkDetail;
};

#endif // NORTHBOUNDMONEYFLOWTABLEWIDGET_H
