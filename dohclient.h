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
    QMap<QNetworkReply*, QByteArray> idReg;

public slots:
    void doDoHQuery(quint16 id, const QByteArray& query);

private slots:
    void onDoHResponse();
    void onErrorOccurred(int code);

signals:
    void onDotFinished(quint16 id, const QByteArray& ans);
    void errorOccurred(const QByteArray& query);
};

#endif // DOHCLIENT_H
