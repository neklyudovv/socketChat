#include "mainwindow.h"

#include <QApplication>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool ok;
    QString name = QInputDialog::getText(nullptr, "socket chat", "enter name:", QLineEdit::Normal, "", &ok);

    if(!ok || name.trimmed().isEmpty())
        return 0;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    std::string nameStr = name.toUtf8().constData();
    send(clientSocket, nameStr.c_str(), nameStr.size(), 0);

    MainWindow w(clientSocket);
    w.show();

    return a.exec();
}
