#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "ui_mainwindow.h"

class QWebSocket;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void onOpened();
    void onClosed();
    void onMessage(const QString &msg);

    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_sendButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::MainWindow ui;
    QWebSocket *webSocket;
};

#endif
