#include "dohclient.h"

#include <Logger.h>

DoHClient::DoHClient(QObject* parent)
    : QObject{ parent }
    , man{ new QNetworkAccessManager{ this } }
{
}

const QString&
DoHClient::getDohServer() const
{
    return dohServer;
}

void
DoHClient::setDohServer(const QString& newDohServer)
{
    dohServer = newDohServer;
}

void
DoHClient::doDoHQuery(quint16 id, const QByteArray& query)
{
    Q_UNUSED(id);
    QNetworkRequest req;
    req.setUrl(QUrl(dohServer));
    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  "application/dns-message");
    auto rep = man->post(req, query);
    connect(rep, &QNetworkReply::finished, this, &DoHClient::onDoHResponse);
    idReg.insert(rep, query);
}

void
DoHClient::onDoHResponse()
{
    auto rep = static_cast<QNetworkReply*>(sender());
    auto code =
        rep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
    if (rep->error() != QNetworkReply::NoError || code != "200") {
        Logger::log("DoHClient",
                    QString("\n\tDoH Answer Error\n\tCode:%1").arg(code));
        this->onErrorOccurred(code.toInt());
        rep->deleteLater();
        return;
    }
    auto ans = rep->readAll();
    quint16 id = ans.at(0);
    id <<= 8;
    id += ans.at(1);
    Logger::log("DoHClient",
                QString("\n\tDoH Answer Received\n\tID:%1").arg(id));
    emit onDotFinished(id, ans);
    idReg.remove(rep);
    rep->deleteLater();
}

void
DoHClient::onErrorOccurred(int code)
{
    Logger::log("DoHClient",
                QString("\n\tDoH Error Occurred!!!\n\tErrMsg:%1").arg(code));
    auto rep = static_cast<QNetworkReply*>(sender());
    emit errorOccurred(idReg[rep]);
    idReg.remove(rep);
}
