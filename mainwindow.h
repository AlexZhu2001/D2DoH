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
    void buildServerClient(const QString& server, Options opt);
    void loadOptions();

private:
    Ui::MainWindow* ui;
    DnsServer* server = nullptr;
    DoHClient* client = nullptr;
};
#endif // MAINWINDOW_H
