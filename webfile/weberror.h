// -------------------------------------------------------------------------------
//  Project:		Qt Webbased File Handling
//
//  File:           weberror.h
//
// (c) 2012 - 2013 by Oblivion Software/Norbert Schlia
// This source code is licensed under The Code Project Open License (CPOL),
// see http://www.codeproject.com/info/cpol10.aspx
// -------------------------------------------------------------------------------
//
#pragma once

#ifndef WEBERROR_H
#define WEBERROR_H

#include <QObject>
#include <QNetworkReply>
#include <QSslError>

class weberror : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("author", "Norbert Schlia")
    Q_CLASSINFO("url", "http://www.oblivion-software.de/")

public:
    explicit weberror(QObject *parent = 0);

    void                        setError(QNetworkReply::NetworkError error);
    QNetworkReply::NetworkError error() const;

    void                        setSslError(QSslError::SslError sslerror);
    QSslError::SslError         errorSsl() const;

    QString                     errorString() const;
    QString                     sslErrorString() const;

signals:

public slots:

protected:
    QNetworkReply::NetworkError m_error;
    QSslError::SslError         m_sslerror;
};

#endif // WEBERROR_H
