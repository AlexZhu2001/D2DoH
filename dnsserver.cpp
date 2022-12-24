#include "dnsserver.h"
#include "Logger.h"
#include <QDateTime>
#include <QNetworkDatagram>
DnsServer::DnsServer(QObject* parent, Options opt)
    : QObject{ parent }
    , socket{ new QUdpSocket{ this } }
    , socketv6{ new QUdpSocket{ this } }
{
    if (opt & OnlyIpv4) {
        socket->bind(QHostAddress::LocalHost, 53, QUdpSocket::ReuseAddressHint);
        connect(socket, &QUdpSocket::readyRead, this, &DnsServer::onDnsQuery);
    }
    if (opt & OnlyIpv6) {
        socketv6->bind(
            QHostAddress::LocalHostIPv6, 53, QUdpSocket::ReuseAddressHint);
        connect(socketv6, &QUdpSocket::readyRead, this, &DnsServer::onDnsQuery);
    }
}

void
DnsServer::onDnsQuery()
{
    auto socket = static_cast<QUdpSocket*>(sender());
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();
        auto data = datagram.data();
        quint16 id = data.at(0);
        id <<= 8;
        id = id + data.at(1);
        AddrPort addrPort{ datagram.senderAddress(), datagram.senderPort() };
        Logger::log("DnsServer",
                    QString("\n\tDns Query Received\n\tID:%1\n\tSrcAddr:%2:%3")
                        .arg(id)
                        .arg(addrPort.addr.toString())
                        .arg(addrPort.port));
        dnsMap.insert(id, addrPort);
        emit DnsReceived(id, data);
    }
}

void
DnsServer::doDnsAnswer(quint16 id, const QByteArray& ans)
{
    QNetworkDatagram datagram;
    datagram.setData(ans);
    auto addr = dnsMap[id].addr;
    auto port = dnsMap[id].port;
    datagram.setDestination(addr, port);
    Logger::log("DnsServer",
                QString("\n\tDoH Answer Received\n\tId:%1\n\tDest:%2:%3")
                    .arg(id)
                    .arg(addr.toString())
                    .arg(port));
    QUdpSocket* socket = nullptr;
    if (addr == QHostAddress::LocalHost) {
        socket = this->socket;
    } else {
        socket = this->socketv6;
    }
    dnsMap.remove(id);
    socket->writeDatagram(datagram);
    socket->flush();
}

void
DnsServer::doErrorReply(quint16 id)
{
    Logger::log("DnsServer", "Error!!!DoH error occurred, Do nothing!!!");
    dnsMap.remove(id);
}
