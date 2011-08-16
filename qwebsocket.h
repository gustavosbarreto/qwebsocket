#ifndef _QWEBSOCKET_H
#define _QWEBSOCKET_H

#include <QObject>

class QWebView;

class QWebSocket: public QObject
{
    Q_OBJECT

public:
    QWebSocket(const QString &address, QObject *parent);

    void send(const QByteArray &data);

private slots:
    void addBridge();

private:
    class Bridge;
    friend class Bridge;

    Bridge *bridge;
    QWebView *webView;

signals:
    void opened();
    void messageReceived(const QString &);
};

#endif
