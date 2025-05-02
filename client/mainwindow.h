#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

#define CLNT_MSG "[Client] "

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int clientSocket, QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newMsgRcv(const QString& message);

private:
    Ui::MainWindow *ui;
    int clientSocket;
    bool run = 1;
    std::thread receive;

private slots:
    void sendMsg();
    void receiveMsg();
};
#endif // MAINWINDOW_H
