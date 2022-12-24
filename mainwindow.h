#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dnsserver.h"
#include "dohclient.h"
#include <QMainWindow>

#define OPTION_FILE "options.json"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void buildServerClient(const QString& server,
                           const QString& dns,
                           Options opt);
    void loadOptions();
    void writeOptions(const QString&, const QString&, Options);

private:
    Ui::MainWindow* ui;
    DnsServer* server = nullptr;
    DoHClient* client = nullptr;
};
#endif // MAINWINDOW_H
