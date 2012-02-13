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
    QUrl url;

public slots:
    QString getUrl() { return url.toString(); }

    void emitOpened()
    {
        emit webSocket->opened();
    }

    void emitMessageReceived(const QString &message)
    {
        emit webSocket->messageReceived(message);
    }
    
    void emitClosed()
    {
        emit webSocket->closed();
    }
    
    void emitError(const QString &message)
    {
        emit webSocket->error(message);
    }
};

QWebSocket::QWebSocket(const QUrl &url, QObject *parent)
    : QObject(parent)
{
    bridge = new Bridge(this);
    bridge->url = url;

    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, false);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, false);

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
