#include "profiles.h"
#include <QDir>
#include <QDebug>
#include "comdatadefines.h"

#define     INI_FILE_NAME           "profiles.ini"

Profiles* Profiles::mInstance = 0;
Profiles::MGarbage Profiles::Garbage;
QMutex Profiles::mutex;

Profiles::Profiles(QObject *parent) :
    mFileSts(false),
    QObject(parent)
{
<<<<<<< HEAD
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
    qDebug()<<__func__<<configSettings->allKeys()<<configSettings->childGroups();

=======
    initContent();
    qDebug()<<"file exist:"<<QFile::exists(INI_FILE_NAME);
>>>>>>> c3de9338bfdafb825be6584d5e9437f142923851
}

Profiles *Profiles::instance()
{
    QMutexLocker locker(&mutex);
    if ( mInstance == 0)
    {
        mInstance = new Profiles();
    }
    return mInstance;
}

void Profiles::save()
{
    qDebug()<<__func__;
    QFile file(INI_FILE_NAME);
    if(file.open(QIODevice::WriteOnly))
    {
        foreach (QString key, mContentList.keys()) {
            file.write(QString("[%1]\n").arg(key).toLocal8Bit());
            foreach (KeyVal data, mContentList[key].mValList) {
                if(data.mVal.type() == QVariant::StringList)
                {
                    file.write(QString("%1=%2").arg(data.mKey).arg(data.mVal.toStringList().join(",")).toLocal8Bit());
                }

            }

        }
        file.close();
        qDebug()<<__func__;
    }

    qDebug()<<__func__;
}

void Profiles::initDefaultExpiredDate()
{
    QString sec = "DateManage";
    QString key = "Expired";
    QStringList list;
    list<<"2017-01-01"<<"2017-01-02"<<"2017-01-03"<<"2017-01-27"<<"2017-01-28"<<"2017-01-29"<<"2017-01-30"<<"2017-01-31"<<"2017-02-01"<<"2017-02-02"<<"2017-04-02"<<"2017-04-03"<<"2017-04-04"<<"2017-04-29"<<"2017-04-30"<<"2017-05-01"<<"2017-05-28"<<"2017-05-29"<<"2017-05-30"<<"2017-10-01"<<"2017-10-02"<<"2017-10-03"<<"2017-10-04"<<"2017-10-05"<<"2017-10-06"<<"2017-10-07"<<"2017-10-08"<<"2018-01-01"<<"2018-02-15"<<"2018-02-16"<<"2018-02-17"<<"2018-02-18"<<"2018-02-19"<<"2018-02-20"<<"2018-02-21"<<"2018-04-05"<<"2018-04-06"<<"2018-04-07"<<"2018-04-29"<<"2018-04-30"<<"2018-05-01"<<"2018-06-16"<<"2018-06-17"<<"2018-06-18"<<"2018-09-22"<<"2018-09-23"<<"2018-09-24"<<"2018-10-01"<<"2018-10-02"<<"2018-10-03"<<"2018-10-04"<<"2018-10-05"<<"2018-10-06"<<"2018-10-07";
    setDefault(sec, key, list);
}

void Profiles::initDefaultFavShare()
{
    Section datesec;
    datesec.mSecName = "ZXG";
    datesec.mCommentlist.append(QStringLiteral("自选股"));
    KeyVal data;
    data.mKey = "codes";
    data.mVal = QStringList()<<"600036"<<"000002"<<"300059"<<"159915"<<"510050";
    datesec.mValList.append(data);
    mContentList[datesec.mSecName] = datesec;
}

void Profiles::initContent()
{
    QFile file(INI_FILE_NAME);
    if(file.open(QIODevice::ReadWrite))
    {
        QByteArray line;
        QStringList commentList;
        QString section;
        QList<KeyVal> valist;
        while ((line = file.readLine()).size() > 0) {
            QString lineContent = QString::fromLocal8Bit(line);
            lineContent = lineContent.trimmed();
            if(lineContent.startsWith("#") || lineContent.startsWith("!"))
            {
                commentList.append(lineContent);
                continue;
            }
            if(lineContent.startsWith("[") && lineContent.endsWith("]"))
            {
                if(section.length() > 0)
                {
                    Section &data = mContentList[section];
                    data.mValList = valist;
                    valist.clear();
                }
                section = lineContent;
                Section &data = mContentList[section];
                data.mSecName = section;
                data.mCommentlist = commentList;
                commentList.clear();
                continue;
            }
            QStringList cols = lineContent.split("=");
            if(cols.length() != 2) continue;
            KeyVal temp;
            temp.mCommentList = commentList;
            temp.mKey = cols[1];
            temp.mVal = cols[2];
            valist.append(temp);
            commentList.clear();
        }
        if(valist.length() > 0)
        {
            Section &data = mContentList[section];
            data.mValList = valist;
            valist.clear();
        }
        mFileSts = true;
        file.close();
    }
    if(mFileSts == true)
    {
        if(mContentList.count() == 0)
        {
            initDefaultExpiredDate();
            initDefaultFavShare();
        }
    }
//    foreach (QString key , mContentList.keys()) {
//        qDebug()<<key<<":"<<mContentList[key].mValList.length();
//        foreach (KeyVal data, mContentList[key].mValList) {
//            qDebug()<<data.mKey<<":"<<data.mVal;
//        }
//    }

//    qDebug()<<"file exist:"<<QFile::exists(INI_FILE_NAME);
}


Profiles::~Profiles()
{
    save();
}

/*-------------------------------------------
 *
 * 设置默认值
 *
---------------------------------------------*/
void Profiles::setDefault(const QString & prefix,const QString &key, const QVariant &value)
{
    Section &data = mContentList[prefix];
    data.mSecName = prefix;
    bool exist = false;
    for(int i=0; i>data.mValList.length(); i++)
    {
        if(data.mValList[i].mKey == key)
        {
            exist = true;
            break;
        }
    }
    if(!exist)
    {
        KeyVal wkdata;
        wkdata.mKey = key;
        wkdata.mVal = value;
        data.mValList.append(wkdata);
    }
    save();

    return;
}
/*-------------------------------------------
 *
 * 设置配置文件值
 *
---------------------------------------------*/
void Profiles::setValue(const QString & prefix,const QString & key, const QVariant & value)
{
    Section &data = mContentList[prefix];
    data.mSecName = prefix;
    bool exist = false;
    for(int i=0; i>data.mValList.length(); i++)
    {
        if(data.mValList[i].mKey == key)
        {
            exist = true;
            data.mValList[i].mVal = value;
            break;
        }
    }
    if(!exist)
    {
        KeyVal wkdata;
        wkdata.mKey = key;
        wkdata.mVal = value;
        data.mValList.append(wkdata);
    }

    save();
    return;
}
/*-------------------------------------------
 *
 * 返回值
 *
---------------------------------------------*/
QVariant Profiles::value(const QString & prefix,const QString &key, const QVariant &defaultValue)
{
    Section &data = mContentList[prefix];
//    qDebug()<<__func__<<data.mSecName<<data.mValList.length();
    data.mSecName = prefix;
    bool exist = false;
    for(int i=0; i<data.mValList.length(); i++)
    {
//        qDebug()<<__func__<<data.mValList[i].mKey<<data.mValList[i].mVal<<key;
        if(data.mValList[i].mKey == key)
        {
            return data.mValList[i].mVal;
        }
    }
    if(!exist)
    {
        KeyVal wkdata;
        wkdata.mKey = key;
        wkdata.mVal = defaultValue;
        data.mValList.append(wkdata);
    }

    return defaultValue;

}

QStringList Profiles::getAllSections()
{
    return mContentList.keys();
}
