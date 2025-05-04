#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(int clientSocket, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), clientSocket(clientSocket)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::sendMsg);
    receive = std::thread(&MainWindow::receiveMsg, this);
}

void MainWindow::receiveMsg(){
    char buffer[1024];
    while(run){
        ssize_t bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytes <= 0) {
            ui->textBrowser->append(CLNT_MSG "lost conn\n");
            run = 0;
        } else{
            buffer[bytes] = '\0';
            ui->textBrowser->append(QString::fromUtf8(buffer));
        }
    }
}

void MainWindow::sendMsg() {
    QString message = ui->textEdit->toPlainText();
    std::string msgStr = message.toStdString();
    ssize_t bytesSent = send(clientSocket, msgStr.c_str(), msgStr.length(), 0);
    if (bytesSent <= 0)
        ui->textBrowser->append(CLNT_MSG "cant send message");
    else {
        ui->textBrowser->append("You: " + message);
        ui->textEdit->clear();
    }
}

MainWindow::~MainWindow()
{
    run = 0;
    ::shutdown(clientSocket, SHUT_RD);
    receive.join();
    ::close(clientSocket);
    delete ui;
}
