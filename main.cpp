#include <QApplication>
#include <QUrl>

#include "qwebsocket.h"

QWebSocket *webSocket;

class Worker: public QObject
{
    Q_OBJECT

public:
    Worker(): QObject() { }

public slots:
    void sendMessage()
    {
        webSocket->send("Hello");
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Worker *w = new Worker;
    webSocket = new QWebSocket(QUrl("ws://echo.websocket.org/"), NULL);
    QObject::connect(webSocket, SIGNAL(opened()), w, SLOT(sendMessage()));

    return app.exec();
}

#include "main.moc"
