#ifndef QGLOBALAPPLICATION_H
#define QGLOBALAPPLICATION_H

#include <QApplication>


#define QDBG qDebug()<<__FILE__<<__FUNCTION__<<__LINE__

class GlobalApplication : public QApplication
{
public:
     GlobalApplication(int&argc,char **argv);
     ~GlobalApplication();

     bool notify(QObject*, QEvent *);
     void setWindowInstance(QWidget*wnd);
private:
     QWidget *widget;
};

#endif // QGLOBALAPPLICATION_H
