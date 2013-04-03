#ifndef _QWEBSOCKET_H
#define _QWEBSOCKET_H

#include <QObject>
#include <QUrl>

class QWebSocketWrapper;

class QWebSocket: public QObject
{
    Q_OBJECT
    public:
    explicit QWebSocket(QObject *parent = 0);
    virtual ~QWebSocket();

    Q_INVOKABLE void connect(const QUrl &uri);
    Q_INVOKABLE void disconnect();

    Q_INVOKABLE void send(const QString &message);

signals:
    void message(const QString &message);
    void opened();
    void closed();
    void failed();

private:
    Q_DISABLE_COPY(QWebSocket)
    QScopedPointer<QWebSocketWrapper> _wrapper;
};

#endif
