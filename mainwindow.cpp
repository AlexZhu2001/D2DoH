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
    connect(actOptions, &QAction::triggered, this, [this]() {
        loadOptions();
        this->show();
    });
    connect(actExit, &QAction::triggered, this, []() { qApp->exit(); });
    connect(ui->btnSave, &QPushButton::clicked, this, [this]() {
        static QRegularExpression exp(
            "https://([-a-zA-Z0-9]*\\.)*([a-zA-Z0-9])*/[-a-zA-Z0-9/]*");
        auto url = ui->edtAddr->text();
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
            QFile f(OPTION_FILE);
            f.open(QFile::WriteOnly | QFile::Truncate);
            QJsonDocument doc;
            QJsonObject obj;
            obj.insert("server", url);
            obj.insert("server-options", (int)opt);
            doc.setObject(obj);
            f.write(doc.toJson());
            f.close();
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
MainWindow::buildServerClient(const QString& server, Options opt)
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
    connect(
        this->server, &DnsServer::DnsReceived, client, &DoHClient::doDotQuery);
    connect(client,
            &DoHClient::onDotFinished,
            this->server,
            &DnsServer::doDnsAnswer);
}

void
MainWindow::loadOptions()
{
    QFile f(OPTION_FILE);
    if (!f.exists()) {
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("server", "https://dns.alidns.com/dns-query");
        obj.insert("server-options", (int)Options::v4Andv6);
        f.open(QFile::WriteOnly | QFile::Truncate);
        doc.setObject(obj);
        f.write(doc.toJson());
        f.close();
    }
    f.open(QFile::ReadOnly);
    auto doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    auto server = doc["server"].toString();
    auto opt = doc["server-options"].toInt();
    this->ui->edtAddr->setText(server);
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
    buildServerClient(server, (Options)opt);
}
