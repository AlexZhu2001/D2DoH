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

    const QString& getBackupDns() const;
    void setBackupDns(const QString& newBackupDns);

private:
    struct AddrPort
    {
        QHostAddress addr;
        int port;
    };
    QUdpSocket* socket;
    QUdpSocket* socketv6;
    QUdpSocket* backupSocket;
    QMap<quint16, AddrPort> dnsMap;
    QString backupDns = "223.5.5.5";

private slots:
    void onDnsQuery();
public slots:
    void doDnsAnswer(quint16 id, const QByteArray& ans);
    void doErrorReply(const QByteArray& query);
signals:
    void DnsReceived(quint16 id, const QByteArray& query);
};

#endif // DNSSERVER_H
