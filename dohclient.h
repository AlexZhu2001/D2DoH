#ifndef DOHCLIENT_H
#define DOHCLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
class DoHClient : public QObject
{
    Q_OBJECT
public:
    explicit DoHClient(QObject* parent = nullptr);

    const QString& getDohServer() const;
    void setDohServer(const QString& newDohServer);

private:
    QNetworkAccessManager* man;
    QString dohServer = "https://dns.alidns.com/dns-query";
    QMap<QNetworkReply*, quint16> idReg;

public slots:
    void doDotQuery(quint16 id, const QByteArray& query);

private slots:
    void onDotResponse();
    void onErrorOccurred(QNetworkReply::NetworkError code);

signals:
    void onDotFinished(quint16 id, const QByteArray& ans);
    void errorOccurred(quint16 id);
};

#endif // DOHCLIENT_H
