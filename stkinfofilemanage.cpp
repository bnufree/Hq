#include "stkinfofilemanage.h"
StkInfoFileManage::StkInfoFileManage(const QString& filenm, QObject *parent) :
    configSettings(NULL),
    QObject(parent)
{
    configSettings = new QSettings(QString("data/%1").arg(filenm), QSettings::IniFormat);
    configSettings->setIniCodec(QTextCodec::codecForName("GB18030"));
}

StkInfoFileManage::~StkInfoFileManage()
{
    if(configSettings)
    {
        configSettings->deleteLater();
    }
}

/*-------------------------------------------
 *
 * 设置默认值
 *
---------------------------------------------*/
void StkInfoFileManage::setDefault(const QString & prefix,const QString &key, const QVariant &value)
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
void StkInfoFileManage::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
    //qDebug()<<"prefix:"<<prefix<<" key:"<<key<<" value:"<<value;
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
QVariant StkInfoFileManage::value(const QString & prefix,const QString &keys, const QVariant &defaultValue)
{
    QVariant values;
    configSettings->beginGroup(prefix);
    {
        values =  configSettings->value( keys,defaultValue);
    }
    configSettings->endGroup();
    return values;
}

QStringList StkInfoFileManage::getAllSections()
{
    return configSettings->childGroups();
}

void StkInfoFileManage::clear()
{
    configSettings->clear();
}

QStringList StkInfoFileManage::subkeys(const QString &prefix)
{
    QStringList keys;
    configSettings->beginGroup(prefix);
    {
        keys =  configSettings->childKeys();
    }
    configSettings->endGroup();
    return keys;
}

void StkInfoFileManage::removeKeys(const QString &prefix, const QStringList &keys)
{
    configSettings->beginGroup(prefix);
    {
        foreach (QString key, keys) {
            configSettings->remove(key);
        }

    }
    configSettings->endGroup();
}

void StkInfoFileManage::removeGroup(const QString &prefix)
{
    removeKeys(prefix, subkeys(prefix));
}
