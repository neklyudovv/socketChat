#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

#define CLNT_MSG "[Client] "

bool run = 1;

void receiveMsg(int clientSocket){
    char buffer[1024];
    while(run){
        ssize_t bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytes <= 0) {
            std::cout << CLNT_MSG << "lost conn\n";
            run = 0;
        } else{
            buffer[bytes] = '\0';
            std::cout << "Client: " << buffer << "\n";
        }
    }
}

void sendMsg(int clientSocket){
    std::string message;
    while(run) {
        std::cout << "You: ";
        std::getline(std::cin, message);
        ssize_t bytesSent = send(clientSocket, message.c_str(), message.size(), 0);
        if (bytesSent <= 0) {
            std::cout << CLNT_MSG << "cant send msg";
            run = 0;
        }
    }
}

int main(){
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    std::thread receive(receiveMsg, clientSocket);
    std::thread send(sendMsg, clientSocket);

    receive.join();
    send.join();
    close(clientSocket);
    return 0;
}