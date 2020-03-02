#ifndef NETWORKCOOKIE_H
#define NETWORKCOOKIE_H

#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>


class NetworkCookie : public QNetworkCookieJar
{
    Q_OBJECT

public:
    NetworkCookie(QObject *parent = 0);
    ~NetworkCookie();

    QList<QNetworkCookie> getCookies();
    void setCookies(const QList<QNetworkCookie>& cookieList);
private:

};

#endif // NETWORKCOOKIE_H
