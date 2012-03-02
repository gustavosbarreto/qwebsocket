#ifndef _HTTPDAEMON_H
#define _HTTPDAEMON_H

#include <QTcpServer>

class HttpDaemon: public QTcpServer
{
    Q_OBJECT

public:
    HttpDaemon();

    void incomingConnection(int socket);

private slots:
    void readClient();
    void discardClient();
};

#endif
