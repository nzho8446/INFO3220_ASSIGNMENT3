#include "server.h"

#include <QDataStream>
#include <QByteArray>
#include "initializer.h"
#include "poolgame.h"


Server::Server(QObject *parent) : QObject(parent),
    server(new QUdpSocket(this)), display(new ServerDisplay())
{

}

#include <QDebug>
void Server::startServer()
{
    //we hard code the port for testing purposes
    server->bind(QHostAddress("192.168.0.6"), 8080);
    qDebug() << connect(server, SIGNAL(readyRead()), this, SLOT(readyRead()));
    display->start();
    pair = QPair<QHostAddress, quint16>(QHostAddress("127.0.0.1"), 6969);
}

void Server::readyRead()
{

    QByteArray buffer;
    buffer.resize(server->pendingDatagramSize());
    QHostAddress sender;
    quint16 port;
    server->readDatagram(buffer.data(), buffer.size(), &sender, &port);
    qDebug() << sender << port;
    QDataStream stream(&buffer, QIODevice::ReadOnly);
    QString command;
    stream >> command;
    qDebug() << command;

    QByteArray data;
    QDataStream writeStream(&data, QIODevice::WriteOnly);
    if (command == "INIT") {
        writeStream << QString("INIT");
        display->serializeGame(writeStream);
        server->writeDatagram(data, sender, port);
        pair = QPair<QHostAddress, quint16>(sender, port);
    } else if (command == "something else") {

    }
}

Server::~Server()
{
    if (server) delete server;
}
