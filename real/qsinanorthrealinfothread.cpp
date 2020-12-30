#include "qsinanorthrealinfothread.h"
#include "utils/qhttpget.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <math.h>

#define         LGT_SH_CODE         "HK_SH"
#define         LGT_SZ_CODE         "HK_SZ"
QSinaNorthRealInfoThread::QSinaNorthRealInfoThread(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<NorthBoundData>("const NorthBoundData&");
    qRegisterMetaType<QList<NorthBoundData> >("const QList<NorthBoundData>&");
}

void QSinaNorthRealInfoThread::run()
{
    while (1) {
        QString url = QString("http://push2.eastmoney.com/api/qt/kamt.rtmin/get?fields1=f1,f3&fields2=f51,f52,f54,f56&ut=b2884a393a59ad64002292a3e90d46a5&_");
        QByteArray http = QHttpGet::getContentOfURL(url.append(QString::number(QDateTime::currentMSecsSinceEpoch())));
        if(http.size() > 0)
        {
            QJsonDocument doc = QJsonDocument::fromJson(http);
            if(doc.isObject())
            {
                QJsonObject total_obj = doc.object();
                if(total_obj.contains("data"))
                {
                    QJsonValue result = total_obj.value("data");
                    if(result.isObject())
                    {
                        QJsonObject obj = result.toObject();
                        QDate now = QDate::fromString(obj.value("s2nDate").toString(), "MM-dd");
                        QJsonValue  data = obj.value("s2n");
                        if(data.isArray())
                        {
                            
                            QList<NorthBoundData> list;
                            int max = 5;
                            int min = 0;
                            QJsonArray array = data.toArray();
                            for(int i=0; i<array.size(); i++)
                            {
                                NorthBoundData north;
                                QStringList strlist = array.at(i).toString().split(",");
                                if(strlist.size() != 4) continue;
                                north.time.setDate(now);
                                north.time.setTime(QTime::fromString(strlist[0], "h:mm"));
                                if(strlist[1] == "-" || strlist[2] == "-") continue;
                                north.sh_flow = strlist[1].toDouble()/10000;
                                north.sz_flow = strlist[2].toDouble()/10000;
                                north.total_flow = north.sh_flow + north.sz_flow;
                                if(north.total_flow > max)
                                {
                                    max = (ceil(north.total_flow) / 2 + 1) *2;
                                }
                                if(north.sh_flow > max)
                                {
                                    max = (ceil(north.sh_flow) / 2 + 1) *2;
                                }
                                if(north.sz_flow > max)
                                {
                                    max = (ceil(north.sz_flow) / 2 + 1) *2;
                                }

                                if(north.total_flow < min)
                                {
                                    min =  (-1) * (ceil(fabs(north.total_flow)) / 2 + 1) *2;
                                }
                                if(north.sh_flow < min)
                                {
                                    min = (-1) * (ceil(fabs(north.sh_flow)) / 2 + 1) *2;
                                }
                                if(north.sz_flow < min)
                                {
                                    min = (-1) * (ceil(fabs(north.sz_flow)) / 2 + 1) *2;
                                }

                                list.append(north);
                            }
                            emit signalUpdateNorthBoundList(list, max, min, now);
                        }
                    }
                }

            }
        }



        QThread::sleep(30);
    }
}
