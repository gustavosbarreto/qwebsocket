#include "httpdaemon.h"

#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>
#include <QFile>

HttpDaemon::HttpDaemon(): QTcpServer()
{
    listen(QHostAddress::Any);
}

void HttpDaemon::incomingConnection(int socket)
{
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);
}

void HttpDaemon::readClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine())
    {
        QTextStream os(socket);
        os.setAutoDetectUnicode(true);
        os << "HTTP/1.0 200 Ok\r\n"
            "Content-Type: text/html; charset=\"utf-8\"\r\n"
            "\r\n";

        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));

        QRegExp pathPattern("^/websocket\\.(html|js)$");
        if (pathPattern.exactMatch(tokens[1]))
        {
            QFile file (":" + pathPattern.capturedTexts()[0]);
            file.open(QFile::ReadOnly);
            os << file.readAll()
               << "\n\n";
        }
        else
        {
            os << "<h1>Nothing to see here</h1>\n\n";
        }

        socket->close();

        if (socket->state() == QTcpSocket::UnconnectedState)
            delete socket;
    }
}

void HttpDaemon::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
