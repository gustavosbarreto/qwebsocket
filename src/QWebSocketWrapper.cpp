#include "QWebSocketWrapper.h"

#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <QDebug>

typedef websocketpp::client<websocketpp::config::asio_client> asio_client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

using websocketpp::lib::bind;

class QWebSocketWrapperPrivate
{
public:
    QWebSocketWrapperPrivate(const QUrl &uri, QWebSocketWrapper *wrapper)
        : _wrapper(wrapper)
        , _uri(uri)
    {
        _endpoint.set_access_channels(websocketpp::log::alevel::all);
        _endpoint.set_error_channels(websocketpp::log::elevel::all);

        // Initialize ASIO
        _endpoint.init_asio();

        // Register our handlers
        _endpoint.set_open_handler(bind(&QWebSocketWrapperPrivate::on_open,this,::_1));
        _endpoint.set_message_handler(bind(&QWebSocketWrapperPrivate::on_message,this,::_1,::_2));
        _endpoint.set_close_handler(bind(&QWebSocketWrapperPrivate::on_close,this,::_1));
        _endpoint.set_fail_handler(bind(&QWebSocketWrapperPrivate::on_fail,this,::_1));
    }

    virtual ~QWebSocketWrapperPrivate()
    {
    }

public:
    void on_fail(websocketpp::connection_hdl hdl)
    {
        qDebug() << "Connection Failed: " << _uri;

        _hdl = hdl;
        _wrapper->failed();
    }

    void on_open(websocketpp::connection_hdl hdl)
    {
        qDebug() << "Connection Opened: " << _uri;

        _hdl = hdl;
        _wrapper->opened();
    }

    void on_close(websocketpp::connection_hdl hdl)
    {
        qDebug() << "Connection Closed: " << _uri;

        _hdl = hdl;
        _wrapper->closed();
    }

    void on_message(websocketpp::connection_hdl, message_ptr msg)
    {
        const QString message = QString::fromUtf8(msg->get_payload().c_str());
        qDebug() << "Got Message:" << message;

        _wrapper->message(message);
    }

    void send(const QString &msg)
    {
        websocketpp::lib::error_code error;
        _endpoint.send(_hdl, msg.toUtf8().data(), websocketpp::frame::opcode::text, error);

        if (error == boost::asio::error::not_connected)
            qDebug() << "Tried to send on a disconnected connection! Aborting.";
    }

    void close()
    {
        websocketpp::lib::error_code error;
        _endpoint.close(_hdl,websocketpp::close::status::going_away,"",error);

        if (error == boost::asio::error::not_connected)
            qDebug() << "Tried to close a disconnected connection!";
    }

    void start()
    {
        websocketpp::lib::error_code error;
        asio_client::connection_ptr conn = _endpoint.get_connection(_uri.toString().toStdString(), error);

        _endpoint.connect(conn);
        _endpoint.run();
    }

    QWebSocketWrapper *_wrapper;
    QUrl _uri;
    asio_client _endpoint;
    websocketpp::connection_hdl _hdl;
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
        _handler->start();
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
