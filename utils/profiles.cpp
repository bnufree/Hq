#include "profiles.h"
#include <QDir>
#include <QDebug>
#include "comdatadefines.h"

#define     INI_FILE_NAME           "profiles.ini"

Profiles* Profiles::minstance = 0;
Profiles::MGarbage Profiles::Garbage;
Profiles::Profiles(QObject *parent) :
    configSettings(NULL),
    QObject(parent)
{
    QString fileNM = QString("%1%2").arg(HQ_WORK_DIR).arg(INI_FILE_NAME);
    bool sts = QFile::exists(fileNM);
    qDebug()<<QFile::exists(fileNM);
    if(!sts)
    {
        QFile file(fileNM);
        if(file.open(QIODevice::WriteOnly))
        {
            file.close();
        }
    }
    qDebug()<<QFile::exists(fileNM);
    configSettings = new QSettings(fileNM, QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GB18030"));
    initDefaultExpiredDate();
    qDebug()<<__func__<<configSettings->allKeys()<<configSettings->childGroups();

}


Profiles::~Profiles()
{
    if(configSettings)
    {
        configSettings->deleteLater();
    }
}
/*-------------------------------------------
 *
 * 实例化
 *
---------------------------------------------*/
Profiles *Profiles::instance()
{
    if ( minstance == 0)
    {
        minstance = new Profiles();
    }
    return minstance;
}
/*-------------------------------------------
 *
 * 设置默认值
 *
---------------------------------------------*/
void Profiles::setDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    configSettings->beginGroup(prefix);
    if(configSettings->value(key).toString().isEmpty())
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}
/*-------------------------------------------
 *
 * 设置配置文件值
 *
---------------------------------------------*/
void Profiles::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
//    qDebug()<<"prefix:"<<prefix<<" key:"<<key<<" value:"<<value;
    configSettings->beginGroup(prefix);
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();
}
/*-------------------------------------------
 *
 * 返回值
 *
---------------------------------------------*/
QVariant Profiles::value(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
    QVariant values;
    configSettings->beginGroup(prefix);
    {
        values =  configSettings->value( keys,defaultValue);
    }
    configSettings->endGroup();
    return values;
}

QStringList Profiles::getAllSections()
{
    qDebug()<<__func__<<QFile::exists(INI_FILE_NAME)<<configSettings->fileName();
    return configSettings->childGroups();
}

void Profiles::initDefaultExpiredDate()
{;
    QStringList list;
    list<<"2017-01-01"<<"2017-01-02"<<"2017-01-03"<<"2017-01-27"<<"2017-01-28"<<"2017-01-29"<<"2017-01-30"<<"2017-01-31"<<"2017-02-01"<<"2017-02-02"<<"2017-04-02"<<"2017-04-03"<<"2017-04-04"<<"2017-04-29"<<"2017-04-30"<<"2017-05-01"<<"2017-05-28"<<"2017-05-29"<<"2017-05-30"<<"2017-10-01"<<"2017-10-02"<<"2017-10-03"<<"2017-10-04"<<"2017-10-05"<<"2017-10-06"<<"2017-10-07"<<"2017-10-08"<<"2018-01-01"<<"2018-02-15"<<"2018-02-16"<<"2018-02-17"<<"2018-02-18"<<"2018-02-19"<<"2018-02-20"<<"2018-02-21"<<"2018-04-05"<<"2018-04-06"<<"2018-04-07"<<"2018-04-29"<<"2018-04-30"<<"2018-05-01"<<"2018-06-16"<<"2018-06-17"<<"2018-06-18"<<"2018-09-22"<<"2018-09-23"<<"2018-09-24"<<"2018-10-01"<<"2018-10-02"<<"2018-10-03"<<"2018-10-04"<<"2018-10-05"<<"2018-10-06"<<"2018-10-07";
    setDefault(CLOSE_DATE_SEC, CLOSE_DATE_KEY, list);
}


