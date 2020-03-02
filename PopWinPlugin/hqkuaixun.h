#ifndef HQKUAIXUN_H
#define HQKUAIXUN_H

#include <QThread>

struct LinkData{
    QString mCode;
    QString mName;
};

struct KuaixunData{
    enum Source{
        Source_EastMoney = 0,
        Source_Ths,
    };

    QString strid;
    QString src_time;
    QString local_time;
    QString title;
    QString url;
    QString digest;
    int     source;
    QList<LinkData> mLinkDataList;
    bool operator <(const KuaixunData &other) const
    {
        if(local_time < other.local_time) return true;
        if(local_time == other.local_time) return src_time < other.src_time;
        return false;
    }

    bool operator >(const KuaixunData &other) const
    {
        if(local_time > other.local_time) return true;
        if(local_time == other.local_time) return src_time > other.src_time;
        return false;
    }
    QString sourceString() const
    {
        if(source == 0) return QStringLiteral("东方财富");
        return QStringLiteral("同花顺");
    }
};


typedef QList<KuaixunData> KuaiXunList;

Q_DECLARE_METATYPE(KuaixunData)
Q_DECLARE_METATYPE(KuaiXunList)

class HqKuaixun : public QThread
{
    Q_OBJECT
public:
    explicit HqKuaixun(QObject *parent = 0);
    void run();
private:
    void parseEastMoney(KuaiXunList& list);
    void parseThs(KuaiXunList& list);
    QList<LinkData> getLinkeDataList(const QString& digest);
signals:
    void signalSendKuaiXun(const KuaiXunList& list);
public slots:
};

#endif // HQKUAIXUN_H
