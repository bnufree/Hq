#ifndef DBSERVICE_H
#define DBSERVICE_H

#include <QObject>
#include <QThread>
#include <QtSql/QSqlDatabase>

#define DATA_SERVICE HqInfoService::instance()

class HqInfoService : public QObject
{
    Q_OBJECT
protected:
    HqInfoService(QObject *parent = 0);
    ~HqInfoService();

public:
    friend class CGarbo;
    static HqInfoService* instance();

signals:

private slots:

private:
    void initSignalSlot();
    bool initDatabase();

private:    //本类使用的变量
    static HqInfoService *m_pInstance;
    class CGarbo        // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~CGarbo()
        {
            //qDebug()<<"delete test obj now";
            if (HqInfoService::m_pInstance)
            {
                delete HqInfoService::m_pInstance;
                HqInfoService::m_pInstance = NULL;
            }
        }
    };
    static CGarbo s_Garbo; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数
    QThread             m_threadWork;       //工作线程
    bool                        mDataBaseInitFlag;
    QSqlDatabase                mDB;
    int                         mCurrentRPLIndex;
};

#endif // DBSERVICE_H
