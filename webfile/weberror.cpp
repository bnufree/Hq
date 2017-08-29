// -------------------------------------------------------------------------------
//  Project:		Qt Webbased File Handling
//
//  File:           weberror.cpp
//
// (c) 2012 - 2013 by Oblivion Software/Norbert Schlia
// This source code is licensed under The Code Project Open License (CPOL),
// see http://www.codeproject.com/info/cpol10.aspx
// -------------------------------------------------------------------------------
//
#include "weberror.h"

weberror::weberror(QObject *parent) :
    QObject(parent)
{
}

void weberror::setError(QNetworkReply::NetworkError error)
{
    m_error = error;
}

QNetworkReply::NetworkError weberror::error() const
{
    return m_error;
}

void weberror::setSslError(QSslError::SslError error)
{
    m_sslerror = error;
}

QSslError::SslError weberror::errorSsl() const
{
    return m_sslerror;
}

QString weberror::errorString() const
{
    QString strErrorString;

    switch (m_error)
    {
    case QNetworkReply::NoError:
        strErrorString = tr("No error");
        break;
    case QNetworkReply::ConnectionRefusedError:
        strErrorString = tr("The remote server refused the connection (the server is not accepting requests).");
        break;
    case QNetworkReply::RemoteHostClosedError:
        strErrorString = tr("The remote server closed the connection prematurely, before the entire reply was received and processed.");
        break;
    case QNetworkReply::HostNotFoundError:
        strErrorString = tr("The remote host name was not found (invalid hostname).");
        break;
    case QNetworkReply::TimeoutError:
        strErrorString = tr("The connection to the remote server timed out.");
        break;
    case QNetworkReply::OperationCanceledError:
        strErrorString = tr("The operation was canceled via calls to abort() or close() before it was finished.");
        break;
    case QNetworkReply::SslHandshakeFailedError:
        strErrorString = tr("The SSL/TLS handshake failed and the encrypted channel could not be established.");
        break;
    case QNetworkReply::TemporaryNetworkFailureError:
        strErrorString = tr("The connection was broken due to disconnection from the network, however the system has initiated roaming to another access point. The request should be resubmitted and will be processed as soon as the connection is re-established.");
        break;
    case QNetworkReply::ProxyConnectionRefusedError:
        strErrorString = tr("The connection to the proxy server was refused (the proxy server is not accepting requests).");
        break;
    case QNetworkReply::ProxyConnectionClosedError:
        strErrorString = tr("The proxy server closed the connection prematurely, before the entire reply was received and processed.");
        break;
    case QNetworkReply::ProxyNotFoundError:
        strErrorString = tr("The proxy host name was not found (invalid proxy hostname).");
        break;
    case QNetworkReply::ProxyTimeoutError:
        strErrorString = tr("The connection to the proxy timed out or the proxy did not reply in time to the request sent.");
        break;
    case QNetworkReply::ProxyAuthenticationRequiredError:
        strErrorString = tr("The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any).");
        break;
    case QNetworkReply::ContentAccessDenied:
        strErrorString = tr("The access to the remote content was denied (similar to HTTP error 401).");
        break;
    case QNetworkReply::ContentOperationNotPermittedError:
        strErrorString = tr("The operation requested on the remote content is not permitted.");
        break;
    case QNetworkReply::ContentNotFoundError:
        strErrorString = tr("The remote content was not found at the server (similar to HTTP error 404).");
        break;
    case QNetworkReply::AuthenticationRequiredError:
        strErrorString = tr("The remote server requires authentication to serve the content but the credentials provided were not accepted (if any).");
        break;
    case QNetworkReply::ContentReSendError:
        strErrorString = tr("The request needed to be sent again, but this failed for example because the upload data could not be read a second time.");
        break;
    case QNetworkReply::ProtocolUnknownError:
        strErrorString = tr("The Network Access API cannot honor the request because the protocol is not known.");
        break;
    case QNetworkReply::ProtocolInvalidOperationError:
        strErrorString = tr("The requested operation is invalid for this protocol.");
        break;
    case QNetworkReply::UnknownNetworkError:
        strErrorString = tr("An unknown network-related error was detected.");
        break;
    case QNetworkReply::UnknownProxyError:
        strErrorString = tr("An unknown proxy-related error was detected.");
        break;
    case QNetworkReply::UnknownContentError:
        strErrorString = tr("An unknown error related to the remote content was detected.");
        break;
    case QNetworkReply::ProtocolFailure:
        strErrorString = tr("A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.).");
        break;
    default:
        strErrorString = QString("Unknown error %1").arg(m_error);
    }

    return strErrorString;
}

// Return localised error string for SSL error
QString weberror::sslErrorString() const
{
    QString strErrorString;

    switch (m_sslerror)
    {
    case QSslError::NoError:
        strErrorString = tr("No error");
        break;
    case QSslError::UnableToGetIssuerCertificate:
        strErrorString = tr("The issuer certificate could not be found.");
        break;
    case QSslError::UnableToDecryptCertificateSignature:
        strErrorString = tr("The certificate signature could not be decrypted.");
        break;
    case QSslError::UnableToDecodeIssuerPublicKey:
        strErrorString = tr("The public key in the certificate could not be read.");
        break;
    case QSslError::CertificateSignatureFailed:
        strErrorString = tr("The signature of the certificate is invalid.");
        break;
    case QSslError::CertificateNotYetValid:
        strErrorString = tr("The certificate is not yet valid.");
        break;
    case QSslError::CertificateExpired:
        strErrorString = tr("The certificate has expired.");
        break;
    case QSslError::InvalidNotBeforeField:
        strErrorString = tr("The certificate's notBefore field contains an invalid time.");
        break;
    case QSslError::InvalidNotAfterField:
        strErrorString = tr("The certificate's notAfter field contains an invalid time.");
        break;
    case QSslError::SelfSignedCertificate:
        strErrorString = tr("The certificate is self-signed, and untrusted.");
        break;
    case QSslError::SelfSignedCertificateInChain:
        strErrorString = tr("The root certificate of the certificate chain is self-signed, and untrusted.");
        break;
    case QSslError::UnableToGetLocalIssuerCertificate:
        strErrorString = tr("The issuer certificate of a locally looked up certificate could not be found.");
        break;
    case QSslError::UnableToVerifyFirstCertificate:
        strErrorString = tr("No certificates could be verified.");
        break;
    case QSslError::InvalidCaCertificate:
        strErrorString = tr("One of the CA certificates is invalid.");
        break;
    case QSslError::PathLengthExceeded:
        strErrorString = tr("The basicConstraints path length parameter has been exceeded");
        break;
    case QSslError::InvalidPurpose:
        strErrorString = tr("The supplied certificate is unsuitable for this purpose.");
        break;
    case QSslError::CertificateUntrusted:
        strErrorString = tr("The root CA certificate is not trusted for this purpose.");
        break;
    case QSslError::CertificateRejected:
        strErrorString = tr("The root CA certificate is marked to reject the specified purpose.");
        break;
    case QSslError::SubjectIssuerMismatch: // hostname mismatch
        strErrorString = tr("The current candidate issuer certificate was rejected because its"
                            " subject name did not match the issuer name of the current certificate.");
        break;
    case QSslError::AuthorityIssuerSerialNumberMismatch:
        strErrorString = tr("The current candidate issuer certificate was rejected because"
                            " its issuer name and serial number was present and did not match the"
                            " authority key identifier of the current certificate.");
        break;
    case QSslError::NoPeerCertificate:
        strErrorString = tr("The peer did not present any certificate.");
        break;
    case QSslError::HostNameMismatch:
        strErrorString = tr("The host name did not match any of the valid hosts"
                            " for this certificate.");
        break;
    case QSslError::UnspecifiedError:
        strErrorString = tr("Unspecified error.");
        break;
    case QSslError::CertificateRevoked:
        strErrorString = tr("The peer presented a certificate that was revoked.");
        break;
    case QSslError::NoSslSupport:
        strErrorString = tr("No SSL support.");
        break;
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 4) // needs Qt 4.7.4 or better
    case QSslError::CertificateBlacklisted:
        strErrorString = tr("The peer presented a certificate that was blacklisted.");
        break;
#endif
    default:
        strErrorString = QString("Unknown SSL error %1").arg(m_sslerror);
    }

    return strErrorString;
}


