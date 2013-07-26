#include "mainwindow.h"

#include <QWebSocket.h>

MainWindow::MainWindow(): QMainWindow()
{
    ui.setupUi(this);

    webSocket = new QWebSocket(this);

    connect(webSocket, SIGNAL(opened()), SLOT(onOpened()));
    connect(webSocket, SIGNAL(closed()), SLOT(onClosed()));
    connect(webSocket, SIGNAL(message(QString)), SLOT(onMessage(QString)));
}

void MainWindow::onOpened()
{
    ui.log->append("<b>CONNECTED</b>");
    ui.connectButton->setEnabled(false);
    ui.disconnectButton->setEnabled(true);
    ui.sendButton->setEnabled(true);
    ui.message->setEnabled(true);
    ui.location->setEnabled(false);
}

void MainWindow::onClosed()
{
    ui.log->append("<b>DISCONNECTED</b>");
    ui.connectButton->setEnabled(true);
    ui.disconnectButton->setEnabled(false);
    ui.sendButton->setEnabled(false);
    ui.message->setEnabled(false);
    ui.location->setEnabled(true);
}

void MainWindow::onMessage(const QString &msg)
{
    ui.log->append(QString("<b>RESPONSE</b>: %1").arg(msg));
}

void MainWindow::on_connectButton_clicked()
{
    webSocket->connect(QUrl(ui.location->text()));
}

void MainWindow::on_disconnectButton_clicked()
{
    webSocket->disconnect();
}

void MainWindow::on_sendButton_clicked()
{
    ui.log->append(QString("<b>SEND:</b> %1").arg(ui.message->text()));
    webSocket->send(ui.message->text());
}

void MainWindow::on_clearButton_clicked()
{
    ui.log->clear();
}
