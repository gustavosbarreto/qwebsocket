QWebSocket
==========

Qt WebSocket library

[![Build Status](https://travis-ci.org/gustavosbarreto/qwebsocket.png?branch=master)](https://travis-ci.org/gustavosbarreto/qwebsocket)

Building
--------

**If you need Qt4 version, checkout the qt4 branch**

    $ git clone --recursive https://github.com/gustavosbarreto/qwebsocket.git
    $ cd qwebsocket
    $ cmake -DCMAKE_INSTALL_PREFIX=/usr
    $ make
    $ make install

Documentation
-------------

# Methods

## connect(QUrl uri)

Connect to a WebSocket server

## disconnect()

Disconnect from the server

## send(QString message)

Send a message to the server

# Signals

## message(QString message)

Emitted when a message arriaves from the server

## opened()

Emitted when the connection to the server is open

## closed()

Emitted when the connection to the server was closed

## failed()

Emitted when the connection to the server has failed
