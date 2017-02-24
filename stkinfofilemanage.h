#ifndef STKINFOFILEMANAGE_H
#define STKINFOFILEMANAGE_H

#include <QObject>
#include <QSettings>
#include <QDebug>
#include <QTextCodec>

class StkInfoFileManage : public QObject
{
    Q_OBJECT
public:
    explicit StkInfoFileManage(const QString& filenm, QObject *parent = 0);
    ~StkInfoFileManage();
    QStringList getAllSections();
private:
    QSettings           *configSettings;
public slots:
    void setValue( const QString & prefix, const QString & key, const QVariant & value );
    void setDefault( const QString & prefix, const QString & key, const QVariant & value );
    QVariant value( const QString & prefix,const QString &keys,const QVariant & defaultValue = QVariant() );
    QStringList subkeys(const QString& prefix);
    void        removeKeys(const QString& prefix, const QStringList& keys);
    void        removeGroup(const QString& prefix);
    void clear();

};
#endif // STKINFOFILEMANAGE_H
