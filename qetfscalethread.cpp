#include "qetfscalethread.h"
#include "utils/qhttpget.h"

QEtfScaleThread::QEtfScaleThread(QObject *parent) : QThread(parent)
{
    qRegisterMetaType<QList<EtfScaleData>>("const QList<EtfScaleData>&");
}


void QEtfScaleThread::run()
{
    // 首先获取上海的规模
    QHttpGet::getContentOfUrl("http://www.sse.com.cn/market/funddata/volumn/etfvolumn/", QString("http://query.sse.com.cn/commonQuery.do?&jsonCallBack=jsonpCallback81067&isPagination=true&sqlId=COMMON_SSE_ZQPZ_ETFZL_XXPL_ETFGM_SEARCH_L&pageHelp.pageSize=10000&STAT_DATE=2020-07-17&_=%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch()));
}
