#include "QWebSocketWrapper.h"

#include <websocketpp.hpp>
#include <websocket_connection_handler.hpp>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <QtCore/QDebug>

using websocketpp::session_ptr;

class QWebSocketWrapperPrivate: public websocketpp::connection_handler
{
public:
    QWebSocketWrapperPrivate(const QUrl &uri, QWebSocketWrapper *wrapper)
        : _wrapper(wrapper)
        , _uri(uri)
    {
    }

    virtual ~QWebSocketWrapperPrivate()
    {
    }

public:
    void on_fail(session_ptr con)
    {
        qDebug() << "Connection Failed: " << _uri;

        _wrapper->failed();
    }

    void on_open(session_ptr con)
    {
        qDebug() << "Connection Opened: " << _uri;

        _session = con;
        _wrapper->opened();
    }

    void on_close(session_ptr con)
    {
        qDebug() << "Connection Closed: " << _uri;

        _session = websocketpp::client_session_ptr();
        _wrapper->closed();
    }

    void on_message(session_ptr con, const std::string &msg)
    {
        const QString message = QString::fromUtf8(msg.c_str());
        qDebug() << "Got Message:" << message;

        _wrapper->message(message);
    }

    virtual void on_message(session_ptr session, const std::vector<unsigned char> &data)
    {
        qWarning() << "The method or operation is not implemented.";
    }

    void send(const QString &msg)
    {
        if (!_session) {
            qDebug() << "Tried to send on a disconnected connection! Aborting.";
            return;
        }

        _session->send(msg.toUtf8().data());
    }

    void close()
    {
        if (!_session) {
            qDebug() << "Tried to close a disconnected connection!";
            return;
        }

        _session->close(websocketpp::close::status::GOING_AWAY,"");
    }

    QWebSocketWrapper *_wrapper;
    QUrl _uri;
    session_ptr _session;
};

QWebSocketWrapper::QWebSocketWrapper(const QUrl &uri, QObject *parent)
    : QThread(parent)
    , _handler(new QWebSocketWrapperPrivate(uri, this))
{
}

QWebSocketWrapper::~QWebSocketWrapper()
{
    if (isRunning()) {
        stop();
        wait(10000);
    }
}

void QWebSocketWrapper::run()
{
    try {
        boost::asio::io_service io_service;

        websocketpp::client_ptr client(
            new websocketpp::client(io_service, boost::shared_ptr<QWebSocketWrapperPrivate>(_handler.data())));

        client->init();
        client->set_header("User Agent","Qt QWebSocket++");

        client->connect(_handler->_uri.toString().toStdString());

        io_service.run();
    } catch(std::exception& e) {
        emit failed();
        qWarning() << "Caught exception trying to get connection to endpoint: " << _handler->_uri << e.what();
    } catch (const char* msg) {
        emit failed();
        qWarning() << "Const const char& exception:" << msg;
    }
}

void QWebSocketWrapper::send(const QString &msg)
{
    Q_ASSERT(!_handler.isNull());
    if (_handler.isNull())
        return;

    _handler->send(msg);
}

void QWebSocketWrapper::stop()
{
    Q_ASSERT(!_handler.isNull());
    if (_handler.isNull())
        return;

    _handler->close();
}
