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

private:
    Ui::MainWindow *ui;
    int clientSocket;

private slots:
    void sendMsg();
};
#endif // MAINWINDOW_H
