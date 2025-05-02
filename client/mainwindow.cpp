#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(int clientSocket, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), clientSocket(clientSocket)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::sendMsg);
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
    ::close(clientSocket);
    delete ui;
}
