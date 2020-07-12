#ifndef CLIENT_H
#define CLIENT_H

#pragma once
//////////////////////////////////////////////////////////////////////////
//¿Í»§¶Ë
#include <QObject>
#include <QAbstractSocket>
#include <QRunnable>
#include <QThreadPool>

class QTcpSocket;
class SimpleTcpSocketClientDemo : public QObject
{
        Q_OBJECT

public:
        SimpleTcpSocketClientDemo();

private slots:
        void connected();
        void readyRead();
        void error(QAbstractSocket::SocketError socketError);

private:
        QTcpSocket* m_pTcpSocket;
};

class ClientRunnable : public QRunnable
{
public:
        void run();
};

void testSimpleTcpSocketClientDemo();


#endif // CLIENT_H
