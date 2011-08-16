#include "qwebsocket.h"

#include <QWebView>
#include <QWebPage>
#include <QWebFrame>

class QWebSocket::Bridge: public QObject
{
    Q_OBJECT

public:
    Bridge(QWebSocket *parent): QObject(parent), webSocket(parent) { }

    QWebSocket *webSocket;
    QString address;

public slots:
    QString getAddress() { return address; }

    void emitOpened()
    {
        emit webSocket->opened();
    }

    void emitMessageReceived(const QString &message)
    {
        emit webSocket->messageReceived(message);
    }
};

QWebSocket::QWebSocket(const QString &address, QObject *parent)
    : QObject(parent)
{
    bridge = new Bridge(this);
    bridge->address = address;

    webView = new QWebView();
    connect(webView->page()->currentFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(addBridge()));
    webView->load(QUrl("qrc:/websocket.html"));
}

void QWebSocket::send(const QByteArray &data)
{
    webView->page()->currentFrame()->evaluateJavaScript("socket.send(\"" + data + "\");");
}

void QWebSocket::addBridge()
{
    webView->page()->currentFrame()->addToJavaScriptWindowObject("Bridge", bridge);
}

#include "qwebsocket.moc"
