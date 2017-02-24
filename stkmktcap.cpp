#include "stkmktcap.h"
MktCapFile* MktCapFile::minstance = 0;
MktCapFile::MGarbage2 MktCapFile::Garbage2;
MktCapFile::MktCapFile(QObject *parent) :
    configSettings(NULL),
    QObject(parent)
{
    configSettings = new QSettings("etc/mktfile.ini", QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GB18030"));
}

MktCapFile::~MktCapFile()
{
    if(configSettings)
    {
        configSettings->deleteLater();
    }
//    if ( minstance )
//    {
//        delete minstance;
//        minstance = 0;
//    }
}
/*-------------------------------------------
 *
 * 实例化
 *
---------------------------------------------*/
MktCapFile *MktCapFile::instance()
{
    if ( minstance == 0)
    {
        minstance = new MktCapFile();
    }
    return minstance;
}
/*-------------------------------------------
 *
 * 设置默认值
 *
---------------------------------------------*/
void MktCapFile::setDefault(const QString & prefix,const QString &key, const QVariant &value)
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
void MktCapFile::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
    qDebug()<<"prefix:"<<prefix<<" key:"<<key<<" value:"<<value;
    configSettings->beginGroup(prefix);
    {
        configSettings->setValue(key, value);
    }
    configSettings->endGroup();    
    configSettings->sync();
}
/*-------------------------------------------
 *
 * 返回值
 *
---------------------------------------------*/
QVariant MktCapFile::value(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
    QVariant values;
    configSettings->beginGroup(prefix);
    {
        values =  configSettings->value( keys,defaultValue);
    }
    configSettings->endGroup();
    return values;
}

QStringList MktCapFile::getAllSections()
{
    return configSettings->childGroups();
}
