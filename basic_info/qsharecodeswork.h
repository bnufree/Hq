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
    void parseShCode();
    void parseSzCode();
    void parseKZZ();
    void parseSzEtf();
    void parseShEtf();
private:
    void parseHttp(ShareDataList& list, const QString& url, int mode = 1); //1:stock 2:fund
    QMap<QString, ShareData>        mShareCodeMap;
};
#endif

#endif // QSHARECODESWORK_H
