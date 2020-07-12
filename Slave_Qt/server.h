#ifndef SOCKET_H
#define SOCKET_H


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//��������-���̴߳���ͻ�������

#include <QTcpServer>
#include <QThread>
class MultiThreadTcpSocketServerDemo : public QTcpServer
{
public:
        MultiThreadTcpSocketServerDemo();
        //This virtual function is called by QTcpServer when a new connection is available.
        //The socketDescriptor argument is the native socket descriptor for the accepted connection.
        virtual void incomingConnection(qintptr handle);

private:
        QStringList m_oData;
};

//�����߳�
class ServerHandleThread : public QThread
{
        Q_OBJECT

public:
        ServerHandleThread(qintptr handle, const QString& sWriteData);

        virtual void run();
private:
        qintptr m_nHandle;
        QString m_sWriteData;
};

void testMultiThreadTcpSocketServerDemo();


#endif // SOCKET_H
