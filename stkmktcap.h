#ifndef STKMKTCAP_H
#define STKMKTCAP_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QTextCodec>

class MktCapFile : public QObject
{
    Q_OBJECT
public:
    ~MktCapFile();
    static MktCapFile *instance();
    QStringList getAllSections();
private:
    explicit MktCapFile(QObject *parent = 0);
private:
    static MktCapFile     *minstance;
    class MGarbage2 // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~MGarbage2()
        {
            if (MktCapFile::minstance)
                delete MktCapFile::minstance;
        }
    };
    static MGarbage2 Garbage2; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

    QSettings           *configSettings;
    QSettings           *iniSettings;
public slots:
    void setValue( const QString & prefix, const QString & key, const QVariant & value );
    void setDefault( const QString & prefix, const QString & key, const QVariant & value );
    QVariant value( const QString & prefix,const QString &keys,const QVariant & defaultValue = QVariant() );

};
#endif // STKMKTCAP_H
