// -------------------------------------------------------------------------------
//  Project:		Qt Webbased File Handling
//
//  File:           webfile.cpp
//
// (c) 2012 - 2013 by Oblivion Software/Norbert Schlia
// This source code is licensed under The Code Project Open License (CPOL),
// see http://www.codeproject.com/info/cpol10.aspx
// -------------------------------------------------------------------------------
//
#define _SHOWBUFFERSTATS

#include "webfile.h"
#include "weberror.h"
#include "debugout.h"

#include <QTimer>
#include <QAuthenticator>
#include <QNetworkProxy>
#include <QThread>
#include <QCoreApplication>

#ifdef _MSC_VER
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__)
#endif

#ifdef _MSC_VER
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#define DEF_OPENTIMEOUT     3000 /* ms */
#define DEF_READTIMEOUT     3000 /* ms */
#define DEF_BUFFERSIZE      (1024*1204) /* = 1 MB */

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0) // Qt 5.0.0
class QThreadEx : public QThread
{
public:
    static void msleep(unsigned long ms)
    {
        QThread::msleep(ms);
    }
};

static void msleep(int ms)
{
    QThreadEx::msleep(ms);
}
#else
static void msleep(int ms)
{
    QThread::msleep(ms);
}
#endif

webfile::webfile(QObject *parent /*= 0*/) :
    QObject(parent),
    m_pSocketThread(NULL),        // Bug #855: fix uninitialised variable
    m_pNetworkProxy(NULL),
    m_pMonitorWindow(NULL)
{
    qRegisterMetaType<READMODE>("READMODE");        // Register our enum as metatype for invokeMethod
    qRegisterMetaType<QByteArray*>("QByteArray*");  // Register QByteArray as metatype for invokeMethod

    clear();
    init();
}

webfile::webfile(const QString & strUrl, QObject *parent /*= 0*/) :
    QObject(parent),
    m_pSocketThread(NULL),        // Bug #855: fix uninitialised variable
    m_pNetworkProxy(NULL),
    m_pMonitorWindow(NULL)
{

    qRegisterMetaType<READMODE>("READMODE");        // Register our enum as metatype for invokeMethod
    qRegisterMetaType<QByteArray*>("QByteArray*");  // Register QByteArray as metatype for invokeMethod

    clear();
    init();
    setUrl(strUrl);
}

webfile::webfile(const QUrl & Url, QObject *parent /*= 0*/) :
    QObject(parent),
    m_pSocketThread(NULL),        // Bug #855: fix uninitialised variable
    m_pNetworkProxy(NULL),
    m_pMonitorWindow(NULL)
{
    clear();
    init();
    setUrl(Url);
}

webfile::~webfile()
{
    // Ensure we exit all loops
    m_loop.exit(1);
    // Close any connection
    close();
    // End socket thread
    endSocketThread();
}

void webfile::endSocketThread()
{
    if (m_pSocketThread != NULL)
    {
        m_pSocketThread->quit();
        m_pSocketThread->wait();

        delete m_pSocketThread;
        m_pSocketThread = NULL;
    }
}

// Reset all variables (e.g for open())
void webfile::clear()
{
    m_pReply            = NULL;
    m_offset            = 0;
    m_nSize             = 0;
    m_bHaveSize         = false;
    m_nPos              = 0;
    m_nReceivedPos      = 0;
    m_nOverHead         = 0;
    m_nReadFails        = 0;
    m_NetworkError      = QNetworkReply::NoError;
    m_nResponse         = 0;
    m_nOpenTimeOutms    = DEF_OPENTIMEOUT;
    m_nReadTimeOutms    = DEF_READTIMEOUT;
    m_bReadTimeOut      = false;
    m_strContentType.clear();
    m_nSize             = 0;
    m_sslerrors.clear();
    m_nBufferSize       = DEF_BUFFERSIZE;
    m_bResetMonitor     = false;
}

void webfile::init()
{
    // Bug #855: fix memory leak
    endSocketThread();

    // QThread is required, otherwise QEventLoop will block
    m_pSocketThread = new QThread(this);
    moveToThread(m_pSocketThread);
    m_pSocketThread->start(QThread::HighestPriority);
}

// The url for open() (similar to file for QFile)
QUrl webfile::url() const
{
    return m_url;
}

void webfile::setUrl(const QString & strUrl)
{
    m_url = QUrl::fromEncoded(strUrl.toUtf8());
}

void webfile::setUrl(const QUrl & Url)
{
    m_url = Url;
}

// Username and password for site, if required
void webfile::setUser(const QString & strUser)
{
    m_strUser = strUser;
}

void webfile::setPassword(const QString & strPassword)
{
    m_strPassword = strPassword;
}

// Actually "open" the file - connect to remote site
bool webfile::open(qint64 offset /*= 0*/)
{
    bool bSuccess = false;

    if (isGuiThread())
    {
        // For GUI threads, we use the non-blocking call and use QEventLoop to wait and yet keep the GUI alive
        QMetaObject::invokeMethod(this, "slotOpen", Qt::QueuedConnection,
                                  Q_ARG(void *, &bSuccess),
                                  Q_ARG(void *, &m_loop),
                                  Q_ARG(qint64, offset));
        m_loop.exec();
    }
    else
    {
        // For non-GUI threads, QEventLoop would cause a deadlock, so we simply use a blocking call.
        // (Does not hurt as no messages need to be processed either during the open operation).
        QMetaObject::invokeMethod(this, "slotOpen", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, &bSuccess),
                                  Q_ARG(void *, NULL),
                                  Q_ARG(qint64, offset));
    }

    return bSuccess;    // Please note that it's perfectly safe to wait on the return Q_ARG, as we wait for the invokeMethod call to complete.
}

void webfile::slotOpen(void *pReturnSuccess, void *pLoop, qint64 offset /*= 0*/)
{
    *(bool*)pReturnSuccess = workerOpen(offset);

    if (pLoop != NULL)
    {
        QMetaObject::invokeMethod((QEventLoop*)pLoop, "quit", Qt::QueuedConnection);
    }
}

bool webfile::workerOpen(qint64 offset /*= 0*/)
{
    //    QDEBUGTS << "start offset" << offset << "bytes at url" << m_url.toString();
    QTime       t;

    t.start();

    clear();

    resetReadFails();

    m_bResetMonitor = true; // Schedule reset

    close();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    if (m_pNetworkProxy != NULL)
    {
        manager->setProxy(*m_pNetworkProxy);
    }

    //connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(replyFinished(QNetworkReply*)));
    connect(manager, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)), SLOT(slotProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));
    connect(manager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), SLOT(slotAuthenticationRequired(QNetworkReply *, QAuthenticator *)));

    QNetworkRequest request;

    request.setUrl(m_url);
    request.setRawHeader("User-Agent", "Qt NetworkAccess 1.3");

    m_nPos = offset;
    if (m_nPos)
    {
        QByteArray data;
        QString strData("bytes=" + QString::number(m_nPos) + "-");

        data = strData.toLatin1();
        request.setRawHeader("Range", data);

        // Range: bytes=20000000-\r\n
        // Range: bytes=0-100\r\n
        // etc.
    }

    m_pReply = manager->get(request);

    if (m_pReply == NULL)
    {
        QDEBUGTS << "network error";
        m_NetworkError = QNetworkReply::UnknownNetworkError;
        return false;
    }

    // Set the read buffer size
    //m_pReply->setReadBufferSize(m_nBufferSize);

    connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(slotError(QNetworkReply::NetworkError)));
    connect(m_pReply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(slotSslErrors(QList<QSslError>)));

    if (!waitForConnect(m_nOpenTimeOutms, manager))
    {
        QDEBUGTS << "connect timeout !!!!!!!!!!!!!!!!!";
        m_NetworkError = QNetworkReply::TimeoutError;
        return false;
    }

    if (m_pReply == NULL)
    {
        QDEBUGTS << "cancelled";
        m_NetworkError = QNetworkReply::OperationCanceledError;
        return false;
    }

    if (m_pReply->error() != QNetworkReply::NoError)
    {
        QDEBUGTS << "error" << m_pReply->errorString();
        m_NetworkError = m_pReply->error();
        return false;
    }

    m_NetworkError      = m_pReply->error();

    m_strContentType    = m_pReply->header(QNetworkRequest::ContentTypeHeader).toString();
    m_LastModified      = m_pReply->header(QNetworkRequest::LastModifiedHeader).toDateTime();
    m_nSize             = m_pReply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    m_nResponse         = m_pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    m_strResponse       = m_pReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

    m_bHaveSize =  (m_nSize ? true : false);

    m_nSize += m_nPos;

    if (error() != QNetworkReply::NoError)
    {
        QDEBUGTS << "error" << error();
        m_NetworkError = m_pReply->error();
        return false;
    }

    m_NetworkError = response2error(m_nResponse);

    QDEBUGTS<<" connect elapsed:"<<t.elapsed();

    return (response() == 200 || response() == 206);
}

void webfile::close()
{
    if (m_pReply != NULL)
    {
        m_pReply->close();
        m_pReply->deleteLater();
        m_pReply = NULL;
    }
}

// File size
qint64 webfile::size() const
{
    if (m_bHaveSize)
    {
        return m_nSize;                     // size as reported by content-length header
    }
    else
    {
        return m_nReceivedPos + m_nSize;    // content-length header was missing, fake size
    }
}

// Current read position
qint64 webfile::pos() const
{
    return m_nPos;
}

// "seek" to position - actually reopen and skip to desired byte offset
bool webfile::seek(qint64 offset)
{
    return open(offset);
}

// Return "true" if file was completely read
bool webfile::atEnd() const
{
    return (pos() >= size() && isFinished());
}

// Read block from file
qint64 webfile::read(char *data, qint64 maxlen)
{
    qint64 nBytesRead = 0;

    if (isGuiThread())
    {
        // For GUI threads, we use the non-blocking call and use QEventLoop to wait and yet keep the GUI alive
        //QEventLoop loop;
        QMetaObject::invokeMethod(this, "slotRead", Qt::QueuedConnection,
                                  Q_ARG(void *, &nBytesRead),
                                  Q_ARG(void *, &m_loop),
                                  Q_ARG(void *, data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READ));
        m_loop.exec();
    }
    else
    {
        // For non-GUI threads, QEventLoop would cause a deadlock, so we simply use a blocking call.
        // (Does not hurt as no messages need to be processed either during the read operation).
        QMetaObject::invokeMethod(this, "slotRead", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, &nBytesRead),
                                  Q_ARG(void *, NULL),
                                  Q_ARG(void *, data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READ));
    }

    return nBytesRead;  // Please note that it's perfectly safe to wait on the return Q_ARG, as we wait for the invokeMethod call to complete.
}

qint64 webfile::readLine(char *data, qint64 maxlen)
{
    qint64 nBytesRead = 0;

    if (isGuiThread())
    {
        // For GUI threads, we use the non-blocking call and use QEventLoop to wait and yet keep the GUI alive
        //QEventLoop loop;
        QMetaObject::invokeMethod(this, "slotRead", Qt::QueuedConnection,
                                  Q_ARG(void *, &nBytesRead),
                                  Q_ARG(void *, &m_loop),
                                  Q_ARG(void *, data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READLINE));
        m_loop.exec();
    }
    else
    {
        // For non-GUI threads, QEventLoop would cause a deadlock, so we simply use a blocking call.
        // (Does not hurt as no messages need to be processed either during the read operation).
        QMetaObject::invokeMethod(this, "slotRead", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, &nBytesRead),
                                  Q_ARG(void *, NULL),
                                  Q_ARG(void *, data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READLINE));
    }

    return nBytesRead;  // Please note that it's perfectly safe to wait on the return Q_ARG, as we wait for the invokeMethod call to complete.
}

QByteArray webfile::read(qint64 maxlen)
{
    QByteArray data;

    if (isGuiThread())
    {
        // For GUI threads, we use the non-blocking call and use QEventLoop to wait and yet keep the GUI alive
        //QEventLoop loop;
        QMetaObject::invokeMethod(this, "slotReadArray", Qt::QueuedConnection,
                                  Q_ARG(void *, &m_loop),
                                  Q_ARG(QByteArray *, &data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READ));
        m_loop.exec();
    }
    else
    {
        // For non-GUI threads, QEventLoop would cause a deadlock, so we simply use a blocking call.
        // (Does not hurt as no messages need to be processed either during the read operation).
        QMetaObject::invokeMethod(this, "slotReadArray", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, NULL),
                                  Q_ARG(QByteArray *, &data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READ));
    }

    return data;  // Please note that it's perfectly safe to wait on the return Q_ARG, as we wait for the invokeMethod call to complete.
}

QByteArray webfile::readAll()
{
    QByteArray data;

    if (isGuiThread())
    {
        // For GUI threads, we use the non-blocking call and use QEventLoop to wait and yet keep the GUI alive
        //QEventLoop loop;
        QMetaObject::invokeMethod(this, "slotReadArray", Qt::QueuedConnection,
                                  Q_ARG(void *, &m_loop),
                                  Q_ARG(QByteArray *, &data),
                                  Q_ARG(qint64, 0),
                                  Q_ARG(READMODE, MODE_READALL));
        m_loop.exec();
    }
    else
    {
        // For non-GUI threads, QEventLoop would cause a deadlock, so we simply use a blocking call.
        // (Does not hurt as no messages need to be processed either during the read operation).
        QMetaObject::invokeMethod(this, "slotReadArray", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, NULL),
                                  Q_ARG(QByteArray *, &data),
                                  Q_ARG(qint64, 0),
                                  Q_ARG(READMODE, MODE_READALL));
    }

    return data;  // Please note that it's perfectly safe to wait on the return Q_ARG, as we wait for the invokeMethod call to complete.
}

QByteArray webfile::readLine(qint64 maxlen /*= 0*/)
{
    QByteArray data;

    if (isGuiThread())
    {
        // For GUI threads, we use the non-blocking call and use QEventLoop to wait and yet keep the GUI alive
        //QEventLoop loop;
        QMetaObject::invokeMethod(this, "slotReadArray", Qt::QueuedConnection,
                                  Q_ARG(void *, &m_loop),
                                  Q_ARG(QByteArray *, &data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READLINE));
        m_loop.exec();
    }
    else
    {
        // For non-GUI threads, QEventLoop would cause a deadlock, so we simply use a blocking call.
        // (Does not hurt as no messages need to be processed either during the read operation).
        QMetaObject::invokeMethod(this, "slotReadArray", Qt::BlockingQueuedConnection,
                                  Q_ARG(void *, NULL),
                                  Q_ARG(QByteArray *, &data),
                                  Q_ARG(qint64, maxlen),
                                  Q_ARG(READMODE, MODE_READLINE));
    }

    return data;  // Please note that it's perfectly safe to wait on the return Q_ARG, as we wait for the invokeMethod call to complete.
}


void webfile::slotRead(void *pResultBytesRead, void *pLoop, void * data, qint64 maxlen, READMODE eReadMode)
{
    *(qint64*)pResultBytesRead = workerRead((char*)data, maxlen, eReadMode);

    if (pLoop != NULL)
    {
        ((QEventLoop*)pLoop)->wakeUp();
        QMetaObject::invokeMethod((QEventLoop*)pLoop, "quit", Qt::QueuedConnection);
    }
}

void webfile::slotReadArray(void *pLoop, QByteArray *data, qint64 maxlen, READMODE eReadMode)
{
    workerReadArray(data, maxlen, eReadMode);

    if (pLoop != NULL)
    {
        ((QEventLoop*)pLoop)->wakeUp();
        QMetaObject::invokeMethod((QEventLoop*)pLoop, "quit", Qt::QueuedConnection);
    }
}

qint64 webfile::workerRead(char *data, qint64 maxlen, READMODE eReadMode)
{
    qint64 nBytesRead = 0;

    if (m_NetworkError != QNetworkReply::NoError)
    {
        return -1;
    }

    m_bReadTimeOut = false;

    try
    {
        qint64 nSize = 0;

        if (m_pReply == NULL)
        {
            QDEBUGTS << "FATAL ERROR! Download cancelled";
            m_NetworkError = QNetworkReply::OperationCanceledError;
            throw (qint64)-1;
        }

        while ((nSize = m_pReply->size()) < maxlen && !isFinished())
        {
            m_nReadFails++; // Wait for more data to arrive

            if (!waitForData(m_nReadTimeOutms))
            {
                QDEBUGTS << "FATAL ERROR! Buffer read timed out";
                if (m_pReply != NULL)
                {
                    m_pReply->close();
                }
                m_NetworkError = QNetworkReply::TimeoutError;
                throw (qint64)-1;
            }
        }

        m_nReceivedPos += nSize - m_nOverHead;

        if (atEnd())
        {
            //QDEBUGTS << "End of file/Stream reached";
            throw (qint64)0;
        }

        if (m_pReply == NULL)
        {
            QDEBUGTS << "FATAL ERROR! Download cancelled";
            m_NetworkError = QNetworkReply::OperationCanceledError;
            throw (qint64)-1;
        }

        switch (eReadMode)
        {
        case MODE_READLINE:
        {
            nBytesRead = m_pReply->readLine(data, maxlen);
            break;
        }
        case MODE_READ:
        {
            nBytesRead = m_pReply->read(data, maxlen);
            break;
        }
        default:
        {
            QDEBUGTS << "FATAL ERROR! Invalid parameter";
            throw (qint64)-1;
            break;
        }
        }

        m_nOverHead = nSize - nBytesRead;
        m_nPos += nBytesRead;
    }
    catch (qint64 _nBytesRead)
    {
        nBytesRead = _nBytesRead;
    }

    return nBytesRead;
}

void webfile::workerReadArray(QByteArray *data, qint64 maxlen, READMODE eReadMode)
{
    data->clear();

    if (m_NetworkError != QNetworkReply::NoError)
    {
        return;
    }

    m_bReadTimeOut = false;

    try
    {
        QDEBUGTS<<" initialize size:"<<m_pReply->bytesAvailable()<<" reply size:"<<m_pReply->size()<<" finish:"<<isFinished();
        if(m_pReply->size())
        {
            *data += m_pReply->readAll();
        }
        while(!isFinished())
        {
            QTime t;
            t.start();
            if(!waitForData(m_nReadTimeOutms))
            {
                QDEBUGTS<<" read timeout!!!!!!!!!!!!!"<<m_url.url()<<" time cost:"<<t.elapsed();
                break;
            }

            *data += m_pReply->readAll();
        }
        //QDEBUGTS<<" total size:"<<data->size();
        return;
        qint64 nSize = 0;

        if (m_pReply == NULL)
        {
            QDEBUGTS << "FATAL ERROR! Download cancelled";
            m_NetworkError = QNetworkReply::OperationCanceledError;
            throw (qint64)-1;
        }

        while ((nSize = m_pReply->size()) < maxlen && !isFinished())
        {
            m_nReadFails++; // Wait for more data to arrive

            if (!waitForData(m_nReadTimeOutms))
            {
                QDEBUGTS << "FATAL ERROR! Buffer read timed out";
                if (m_pReply != NULL)
                {
                    m_pReply->close();
                }
                m_NetworkError = QNetworkReply::TimeoutError;
                throw (qint64)-1;
            }
        }

        m_nReceivedPos += nSize - m_nOverHead;

        if (atEnd())
        {
            //QDEBUGTS << "End of file/Stream reached";
            throw (qint64)0;
        }

        if (m_pReply == NULL)
        {
            QDEBUGTS << "FATAL ERROR! Download cancelled";
            m_NetworkError = QNetworkReply::OperationCanceledError;
            throw (qint64)-1;
        }

        switch (eReadMode)
        {
        case MODE_READLINE:
        {
            *data = m_pReply->readLine(maxlen);
            break;
        }
        case MODE_READ:
        {
            *data = m_pReply->read(maxlen);
            break;
        }
        case MODE_READALL:
        {
            *data = m_pReply->readAll();
            break;
        }
        default:
        {
            QDEBUGTS << "FATAL ERROR! Invalid parameter";
            throw (qint64)-1;
            break;
        }
        }

        m_nOverHead = nSize - data->count();
        m_nPos += data->count();
    }
    catch (qint64)
    {
        data->clear();
    }
}

// Return NetworkError code
QNetworkReply::NetworkError webfile::error() const
{
    return m_NetworkError;
}

// Return localised error string for network error (default: current code)
QString webfile::errorString(QNetworkReply::NetworkError networkerror /*= QNetworkReply::NoError*/) const
{
    if (networkerror == QNetworkReply::NoError)
    {
        networkerror = error();
    }

    weberror we;

    we.setError(networkerror);

    return we.errorString();
}

// Response code (200, 404, etc.)
int webfile::response() const
{
    return m_nResponse;
}

// Corresponding text to response code
QString webfile::responseString() const
{
    return m_strResponse;
}

// true if download finished
bool webfile::isFinished() const
{
    if (m_pReply != NULL)
    {
        return m_pReply->isFinished();
    }
    else
    {
        return true;
    }
}

// true if download still running
bool webfile::isRunning() const
{
    if (m_pReply != NULL)
    {
        return m_pReply->isRunning();
    }
    else
    {
        return false;
    }
}

bool webfile::isThreadRunning() const
{
    if (m_pSocketThread != NULL)
    {
        return m_pSocketThread->isRunning();
    }
    else
    {
        return false;
    }
}

// Number of "read fails", e.g., not enough data available as requested
int webfile::getReadFails() const
{
    return m_nReadFails;
}

void webfile::resetReadFails()
{
    m_nReadFails = 0;
}

// Set the current proxy. Must be done before open(), will not have effect otherwise
void webfile::setProxy(const QNetworkProxy * networkProxy)
{
    m_pNetworkProxy = networkProxy;
}

// Receive network error
void webfile::slotError(QNetworkReply::NetworkError error)
{

    m_NetworkError = error;

    QDEBUGTS << "error" << error << errorString();
}

// Receive SSL errors
void webfile::slotSslErrors(QList<QSslError> sslerrors)
{
    QDEBUGTS << "start";

    m_sslerrors.append(sslerrors);

#ifdef QT_DEBUG
    for (int i = 0; i < sslerrors.size(); i++)
    {
        QDEBUGTS << sslerrors[i].errorString();
    }
#endif

    QDEBUGTS << "done";
}

// Do proxy authentication
void webfile::slotProxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator)
{
    //QDEBUGTS << "Proxy authentication required" << proxy.hostName();

    //    if (!proxy.user().isEmpty() && !proxy.password().isEmpty() &&
    //            (authenticator->user() != proxy.user() ||
    //            authenticator->password() != proxy.password()))
    {
        // Send credentials if changed and not empty
        authenticator->setUser(proxy.user());
        authenticator->setPassword(proxy.password());
    }

    // QNetworkProxy p(proxy);
    // p.setUser(strUser);
    // p.setPassword(strPassword);
    // QNetworkProxy::setApplicationProxy(p);
}

// Do remote server authentication
void webfile::slotAuthenticationRequired(QNetworkReply * /*reply*/, QAuthenticator * authenticator)
{
    //QDEBUGTS << "Authentication required for realm" << authenticator->realm(); // << "at url" << reply->url();

    if (!m_strUser.isEmpty() && !m_strPassword.isEmpty() &&
            (authenticator->user() != m_strUser ||
             authenticator->password() != m_strPassword))
    {
        // Send credentials if changed and not empty
        authenticator->setUser(m_strUser);
        authenticator->setPassword(m_strPassword);
    }
}

// Wait timeout
void webfile::slotWaitTimeout()
{
    QDEBUGTS << "START";

    m_bReadTimeOut = true;  // Report timeout
    emit signalReadTimeout();
}

// Open timeout
int webfile::getOpenTimeout() const
{
    return m_nOpenTimeOutms;
}

void webfile::setOpenTimeout(int nOpenTimeOutms)
{
    m_nOpenTimeOutms = nOpenTimeOutms;
}

// Read timeout
int webfile::getReadTimeout() const
{
    return m_nReadTimeOutms;
}

void webfile::setReadTimeout(int nReadTimeOutms)
{
    m_nReadTimeOutms = nReadTimeOutms;
}

// Internal: wait for connection to establish
bool webfile::waitForConnect(int nTimeOutms, QNetworkAccessManager *manager)
{
    QTimer *timer = NULL;
    QEventLoop eventLoop;
    bool bReadTimeOut = false;

    m_bReadTimeOut = false;

    if (nTimeOutms > 0)
    {
        timer = new QTimer(this);

        connect(timer, SIGNAL(timeout()), this, SLOT(slotWaitTimeout()));
        timer->setSingleShot(true);
        timer->start(nTimeOutms);

        connect(this, SIGNAL(signalReadTimeout()), &eventLoop, SLOT(quit()));
    }

    // Wait on QNetworkManager reply here
    connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));

    if (m_pReply != NULL)
    {
        // Preferrably we wait for the first reply which comes faster than the finished signal
        connect(m_pReply, SIGNAL(readyRead()), &eventLoop, SLOT(quit()));
    }
    eventLoop.exec();

    if (timer != NULL)
    {
        timer->stop();
        delete timer;
        timer = NULL;
    }

    bReadTimeOut = m_bReadTimeOut;

    m_bReadTimeOut = false;

    return !bReadTimeOut;
}

// Internal: wait for new data to arrive
bool webfile::waitForData(int nTimeOutms)
{
    QEventLoop  eventLoop;
    QTimer *    timer = NULL;
    bool        bReadTimeOut = false;

    m_bReadTimeOut = false;

    if (nTimeOutms > 0)
    {
        timer = new QTimer(this);	// ??? owner??

        connect(timer, SIGNAL(timeout()), this, SLOT(slotWaitTimeout()));
        timer->setSingleShot(true);
        timer->start(nTimeOutms);

        connect(this, SIGNAL(signalReadTimeout()), &eventLoop, SLOT(quit()));
    }

    if (m_pReply != NULL)
    {
        connect(m_pReply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
        connect(m_pReply, SIGNAL(readyRead()), &eventLoop, SLOT(quit()));
    }

    eventLoop.exec();

    if (timer != NULL)
    {
        timer->stop();
        delete timer;
        timer = NULL;
    }

    bReadTimeOut = m_bReadTimeOut;

    m_bReadTimeOut = false;

    return !bReadTimeOut;
}

// Return content type of document, if available
QString webfile::getContentType() const
{
    return m_strContentType;
}

// Return last modified time of document, if available
QDateTime webfile::getLastModified() const
{
    return m_LastModified;
}

// Internal: detect if running as GUI thread
bool webfile::isGuiThread() const
{
    QCoreApplication *pCoreApp = QCoreApplication::instance();

    if (pCoreApp == NULL)
    {
        return false;       // No QApp object, should never happen, but no Qt GUI anyway.
    }
    return (QThread::currentThread() == pCoreApp->thread());
}

QNetworkReply * webfile::getReply()
{
    return m_pReply;
}

QNetworkReply::NetworkError webfile::response2error(int nResponse) const
{
    QNetworkReply::NetworkError error = QNetworkReply::NoError;

    switch (nResponse)
    {
    case 200:
    case 206:
        break;
    case 401:
        error = QNetworkReply::AuthenticationRequiredError;
        break;
    case 403:
        error = QNetworkReply::ContentOperationNotPermittedError;
        break;
    case 404:
        error = QNetworkReply::ContentNotFoundError;
        break;
    default:
        error = QNetworkReply::UnknownNetworkError;
        break;
    }

    return error;
}

void  webfile::setReadBufferSize(qint64 size)
{
    if (m_pReply != NULL)
    {
        m_pReply->setReadBufferSize(size);
    }
}

qint64 webfile::readBufferSize() const
{
    if (m_pReply != NULL)
    {
        return m_pReply->readBufferSize();
    }
    else
    {
        return -1;
    }
}

void webfile::wait() const
{
    while (m_pMonitorWindow != NULL)
    {
        msleep(20);
    }
}
