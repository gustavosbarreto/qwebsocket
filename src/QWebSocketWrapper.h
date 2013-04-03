#ifndef _QWEBSOCKETWRAPPER_H
#define _QWEBSOCKETWRAPPER_H

#include <QtCore/QThread>
#include <QUrl>

class QWebSocketWrapperPrivate;

class QWebSocketWrapper : public QThread
{
    Q_OBJECT
    public:
    explicit QWebSocketWrapper(const QUrl &uri, QObject *parent = 0);
    virtual ~QWebSocketWrapper();

    void send(const QString &msg);
    void stop();

signals:
    void message(const QString &msg);

    void opened();
    void closed();
    void failed();

protected:
    void run();

private:
    Q_DISABLE_COPY(QWebSocketWrapper)

    QScopedPointer<QWebSocketWrapperPrivate> _handler;
    friend class QWebSocketWrapperPrivate;
};

#endif
