#ifndef DNSSERVER_H
#define DNSSERVER_H

#include "common.h"
#include <QMap>
#include <QObject>
#include <QUdpSocket>
class DnsServer : public QObject
{
    Q_OBJECT
public:
    explicit DnsServer(QObject* parent = nullptr, Options opt = OnlyIpv4);

private:
    struct AddrPort
    {
        QHostAddress addr;
        int port;
    };
    QUdpSocket* socket;
    QUdpSocket* socketv6;
    QMap<quint16, AddrPort> dnsMap;

private slots:
    void onDnsQuery();
public slots:
    void doDnsAnswer(quint16 id, const QByteArray& ans);
    void doErrorReply(quint16 id);
signals:
    void DnsReceived(quint16 id, const QByteArray& query);
};

#endif // DNSSERVER_H
