#ifndef QHTTPGET_H
#define QHTTPGET_H

#include <QObject>

class QHttpGet : public QObject
{
    Q_OBJECT
public:
    explicit QHttpGet(const QString& url = QString(), QObject *parent = 0);
    ~QHttpGet();
    QByteArray getContent(const QString& url = QString());
    static QByteArray getContentOfURL(const QString& url);

signals:

public slots:
private:
    QString     mUrl;

};

#endif // QHTTPGET_H
