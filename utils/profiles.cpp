#include "profiles.h"
#include <QDir>
#include <QDebug>
#include "comdatadefines.h"
#include "data_structure/hqutils.h"

#define     INI_FILE_NAME           "profiles.ini"

Profiles* Profiles::minstance = 0;
//Profiles::MGarbage Profiles::Garbage;
Profiles::Profiles(QObject *parent) :
    configSettings(NULL),
    QObject(parent)
{
    QString fileNM = QString("%1/%2").arg(HQ_CFG_DIR).arg(INI_FILE_NAME);
    bool sts = QFile::exists(fileNM);
    if(!sts)
    {
        HqUtils::makePath(HQ_CFG_DIR);
        QFile file(fileNM);
        if(file.open(QIODevice::WriteOnly))
        {
            file.close();
        }
    }
    qDebug()<<QFile::exists(fileNM);
    configSettings = new QSettings(fileNM, QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GB18030"));
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

