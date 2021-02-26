#ifndef QSHARECODESWORK_H
#define QSHARECODESWORK_H

#include <QRunnable>
#include <QObject>
#include <QThread>
#include "data_structure/sharedata.h"

#ifdef HQ_RUNNABLE
class QShareCodesWork : public QRunnable
{
public:
    explicit QShareCodesWork(QObject *parent = 0);
    ~QShareCodesWork();
    void run();
private:
    QObject* mParent;
};
#else
class QShareCodesWork : public QThread
{
    Q_OBJECT
public:
    explicit QShareCodesWork(QObject *parent = 0);
    ~QShareCodesWork();
    void run();
    int parseShCode();
    int parseSzCode();
    int parseKZZ();
    int parseSzEtf();
    int parseShEtf();
private:
    void parseHttp(const QString& url); //1:stock 2:fund
    QMap<QString, ShareData>        mShareCodeMap;
};
#endif

#endif // QSHARECODESWORK_H
