#include "test_fakeserver.h"

#include <QTcpSocket>

FakeServer::FakeServer(int port)
{
    m_port = port;
    connect(this, &FakeServer::newConnection, this, &FakeServer::createNewConnection);
}

void FakeServer::startListen()
{
    if (!isListening()) {
        listen(QHostAddress::Any, m_port);
    }
}

void FakeServer::stopListen()
{
    close();
}

void FakeServer::setRawAnswer(const QByteArray &rawAnswer)
{
    m_rawAnswer = rawAnswer;
}

void FakeServer::createNewConnection()
{
    if (hasPendingConnections()) {
        QTcpSocket* socket = nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(sendData()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(removeConnection()));
    }
}

void FakeServer::removeConnection()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void FakeServer::sendData()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (tokens[0] == "GET" || tokens[0] == "POST") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);
            os << "HTTP/1.0 200 Ok\r\n"
                  "Content-Type: text/json; charset=\"utf-8\"\r\n"
                  "\r\n"
               << m_rawAnswer << "\n";
            socket->close();

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
            }
        }
    }
}