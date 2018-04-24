#ifndef PROFILE_H
#define PROFILE_H

#include <QObject>
#include <QDebug>
#include <QTextCodec>
#include <QFile>
#include <QMutex>

struct KeyVal
{
    QString     mKey;
    QVariant    mVal;
    QStringList mCommentList;
};

struct Section
{
    QString             mSecName;
    QList<KeyVal>       mValList;
    QStringList         mCommentlist;
};


class Profiles : public QObject
{
    Q_OBJECT
public:
    ~Profiles();
    QStringList getAllSections();
    static Profiles* instance();

private:
    explicit Profiles(QObject *parent = 0);
    static QMutex      mutex;
    void initContent();
    void initDefaultExpiredDate();
    void initDefaultFavShare();
    void save();
    static Profiles     *mInstance;
private:
    class MGarbage // 它的唯一工作就是在析构函数中删除CSingleton的实例
    {
    public:
        ~MGarbage()
        {
            if (Profiles::mInstance)
                delete Profiles::mInstance;
        }
    };
    static MGarbage Garbage; // 定义一个静态成员，在程序结束时，系统会调用它的析构函数

    QMap<QString, Section>      mContentList;
    bool                        mFileSts;
public slots:
    void setValue( const QString & prefix, const QString & key, const QVariant & value );
    void setDefault( const QString & prefix, const QString & key, const QVariant & value );
    QVariant value( const QString & prefix,const QString &keys,const QVariant & defaultValue = QVariant() );

};
#endif // PROFILE_H
