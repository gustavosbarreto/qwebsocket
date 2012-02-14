#include <QApplication>
#include <QUrl>
#include <QMessageBox>

#include "ui_echotest.h"
#include "qwebsocket.h"

class EchoTest: public QWidget
{
    Q_OBJECT

public:
    EchoTest(): QWidget()
    {
        ui.setupUi(this);

        connect(ui.connect, SIGNAL(clicked()), SLOT(doConnect()));
        connect(ui.disconnect, SIGNAL(clicked()), SLOT(doDisconnect()));
        connect(ui.send, SIGNAL(clicked()), SLOT(doSend()));
        connect(ui.clearLog, SIGNAL(clicked()), ui.log, SLOT(clear()));
    }

private slots:
    void doConnect()
    {
        webSocket = new QWebSocket(QUrl(ui.location->text()), NULL);
        connect(webSocket, SIGNAL(opened()), SLOT(onOpen()));
        connect(webSocket, SIGNAL(closed()), SLOT(onClose()));
        connect(webSocket, SIGNAL(error(const QString &)), SLOT(onError(const QString &)));
        connect(webSocket, SIGNAL(messageReceived(const QString &)), SLOT(onMessage(const QString &)));
    }

    void doDisconnect()
    {
        QMessageBox::warning(this, "Warning", "Not implemented yet!");
    }

    void doSend()
    {
        ui.log->appendPlainText(QString("SENT: %1").arg(ui.message->text()));;
        webSocket->send(ui.message->text().toUtf8());
    }

    void onOpen()
    {
        ui.log->appendPlainText("CONNECTED");
        ui.connect->setEnabled(false);
        ui.disconnect->setEnabled(true);
        ui.message->setEnabled(true);
        ui.send->setEnabled(true);
    }

    void onMessage(const QString &message)
    {
        ui.log->appendPlainText(QString("RESPONSE: %1").arg(message));;
    }

    void onClose()
    {
        ui.log->appendPlainText("DISCONNECTED");
        ui.connect->setEnabled(true);
        ui.disconnect->setEnabled(false);
        ui.message->setEnabled(false);
        ui.send->setEnabled(false);

        webSocket->deleteLater();
        webSocket = NULL;
    }

    void onError(const QString &message)
    {
        // nothing
    }

private:
    Ui::EchoTest ui;
    QWebSocket *webSocket;
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    (new EchoTest)->show();

    return app.exec();
}

#include "echotest.moc"
