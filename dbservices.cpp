#include <QDebug>
#include <QSharedPointer>
#include <QSqlError>
#include <QDir>
#include "dbservices.h"

HqInfoService* HqInfoService::m_pInstance = 0;
HqInfoService::CGarbo HqInfoService::s_Garbo;

HqInfoService::ZCHXDataChangeService(QObject *parent) :
    QObject(parent),
    mDataBaseInitFlag(0)
{
    bool sts = initDatabase();
    qDebug()<<"init db status:"<<sts;
    initSignalSlot();
    //3、开启异步通讯
    moveToThread(&m_threadWork);
    m_threadWork.start();
}

HqInfoService::~HqInfoService()
{
    if(mDataBaseInitFlag)
    {
        mDB.close();
    }
    m_threadWork.quit();
    m_threadWork.wait(500);
    m_threadWork.terminate();
}

void HqInfoService::initSignalSlot()
{

}


HqInfoService *HqInfoService::instance()
{
    if(m_pInstance == 0)
    {
        m_pInstance = new HqInfoService();
    }
    return m_pInstance;
}


bool HqInfoService::initDatabase()
{
    if(mDataBaseInitFlag) return true;
    //初始化本地数据库的连接
    mDB = QSqlDatabase::addDatabase("QSQLITE");//链接数据库
    mDB.setDatabaseName("sccmms_se");
    return mDB.open();
}
