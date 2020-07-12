#include "server.h"
#include <assert.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>
#include "SendMes.h"


server::server()
{
        //��ʼ��ԭʼ����
        m_oData = read.buffer_send;
                                        
        //1. ����TCP����
        m_pTcpServer = new QTcpServer(this);
        //2. �����ӡ������ź�
        connect(m_pTcpServer, &QTcpServer::newConnection, this, &server::sendData);
        connect(m_pTcpServer, &QTcpServer::acceptError, this, &server::displayError);
        //3. ���������
        if (!m_pTcpServer->listen(QHostAddress::Any, 8888))
        {
                qDebug() << "m_pTcpServer->listen() error";
                assert(false);
        }
}

void server::sendData()
{
        //��ȡ���������
        QString sWriteData = m_oData;
        //��ȡ��ͻ���ͨ�ŵ�socket
        QTcpSocket* pClientConnection = m_pTcpServer->nextPendingConnection();
        //�ӿͻ��˶�����
        QString sReadData = pClientConnection->readAll();
        qDebug() << "server::readDataFromClient " << pClientConnection;
        //��ͻ���д����
        qDebug() << "server::writeDataToClient " << sWriteData;
        pClientConnection->write(sWriteData.toUtf8());
// 	//��ͻ��˶Ͽ�����
// 	connect(pClientConnection, &QTcpSocket::disconnected, this, &server::deleteLater);
// 	pClientConnection->disconnectFromHost();
}

void server::displayError(QAbstractSocket::SocketError socketError)
{
        qDebug() << "server::displayError " << socketError;
}

void testserver()
{
    //����д���ڴ�й©�����д������ԡ�
        server* pSimpleTcpSocketServer = new server;
}

