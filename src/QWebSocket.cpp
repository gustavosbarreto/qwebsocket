#include "QWebSocket.h"
#include "QWebSocketWrapper.h"

QWebSocket::QWebSocket(QObject *parent):
    QObject(parent)
{
}

QWebSocket::~QWebSocket()
{
}

void QWebSocket::connect(const QUrl &uri)
{
    _wrapper.reset(new QWebSocketWrapper(uri, this));

    QObject::connect(_wrapper.data(), SIGNAL(message(const QString&)), SIGNAL(message(const QString&)));
    QObject::connect(_wrapper.data(), SIGNAL(opened()), SIGNAL(opened()));
    QObject::connect(_wrapper.data(), SIGNAL(closed()), SIGNAL(closed()));
    QObject::connect(_wrapper.data(), SIGNAL(failed()), SIGNAL(failed()));

    _wrapper->start();
}

void QWebSocket::disconnect()
{
    if (_wrapper.isNull())
        return;

    _wrapper->stop();
    _wrapper.reset();
}

void QWebSocket::send(const QString &message)
{
    if (_wrapper.isNull())
        return;

    _wrapper->send(message);
}
