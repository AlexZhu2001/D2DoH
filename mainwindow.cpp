#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMessageBox>
#include <QRegularExpression>
#include <QStyle>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIcon iconPic(":/img/doh.png");
    this->setWindowIcon(iconPic);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setWindowTitle(tr("Options"));
    auto icon = new QSystemTrayIcon(this);
    auto menu = new QMenu();
    auto actOptions = new QAction(tr("Open Options"));
    auto actExit = new QAction(tr("Exit"));
    menu->addAction(actOptions);
    menu->addAction(actExit);
    icon->setIcon(iconPic);
    icon->setContextMenu(menu);
    icon->show();
    loadOptions();
    connect(actOptions, &QAction::triggered, this, [this]() {
        loadOptions();
        this->show();
    });
    connect(actExit, &QAction::triggered, this, []() { qApp->exit(); });
    connect(ui->btnSave, &QPushButton::clicked, this, [this]() {
        static QRegularExpression exp(
            "https://([-a-zA-Z0-9]*\\.)*([a-zA-Z0-9])*/[-a-zA-Z0-9/]*");
        auto url = ui->edtAddr->text();
        auto dns = ui->edtDnsAddr->text();
        auto v4 = ui->chkEnV4->isChecked();
        auto v6 = ui->chkEnV6->isChecked();
        if (!exp.match(url).hasMatch()) {
            QMessageBox::critical(this, tr("Error"), tr("Server is invalid"));
            return;
        } else if ((!v4) && (!v6)) {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("You should enable at least one options"));
            return;
        } else {
            Options opt;
            if (v4 && v6) {
                opt = v4Andv6;
            } else if (v6) {
                opt = OnlyIpv6;
            } else {
                opt = OnlyIpv4;
            }
            writeOptions(url, dns, opt);
            loadOptions();
            this->close();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::buildServerClient(const QString& server,
                              const QString& dns,
                              Options opt)
{
    if (this->server != nullptr) {
        delete this->server;
    }
    if (this->client != nullptr) {
        delete this->client;
    }
    this->server = new DnsServer(this, opt);
    this->client = new DoHClient(this);
    client->setDohServer(server);
    this->server->setBackupDns(dns);
    connect(
        this->server, &DnsServer::DnsReceived, client, &DoHClient::doDoHQuery);
    connect(client,
            &DoHClient::onDotFinished,
            this->server,
            &DnsServer::doDnsAnswer);
    connect(client,
            &DoHClient::errorOccurred,
            this->server,
            &DnsServer::doErrorReply);
}

void
MainWindow::loadOptions()
{
    QFile f(OPTION_FILE);
    if (!f.exists()) {
        writeOptions(
            "https://dns.alidns.com/dns-query", "223.5.5.5", Options::v4Andv6);
    }
    f.open(QFile::ReadOnly);
    auto doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    auto server = doc["server"].toString();
    auto dns = doc["backdns"].toString();
    auto opt = doc["server-options"].toInt();

    this->ui->edtAddr->setText(server);
    this->ui->edtDnsAddr->setText(dns);
    if (opt & Options::OnlyIpv4) {
        this->ui->chkEnV4->setChecked(true);
    } else {
        this->ui->chkEnV4->setChecked(false);
    }
    if (opt & Options::OnlyIpv6) {
        this->ui->chkEnV6->setChecked(true);
    } else {
        this->ui->chkEnV6->setChecked(false);
    }
    buildServerClient(server, dns, (Options)opt);
}

void
MainWindow::writeOptions(const QString& url, const QString& dns, Options opt)
{
    QFile f(OPTION_FILE);
    f.open(QFile::WriteOnly | QFile::Truncate);
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("server", url);
    obj.insert("backdns", dns);
    obj.insert("server-options", (int)opt);
    doc.setObject(obj);
    f.write(doc.toJson());
    f.close();
}
