#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        std::cout << "establishing conn error";
        exit(0);
    }

    std::cout << "socket for server has been created" << std::endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (ret < 0) {
        std::cout << "error binding conn" << std::endl;
        return -1;
    }

    listen(serverSocket, 5);

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    char buffer[1024] = {0};
    ssize_t bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
    while(bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << "Client: " << buffer << std::endl;
        bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
    }

    close(serverSocket);
    return 0;
}