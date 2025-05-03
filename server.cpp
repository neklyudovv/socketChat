#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>

#define SERV_MSG "[SERVER] "

int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        std::cout << SERV_MSG << "establishing conn error";
        exit(0);
    }

    std::cout << SERV_MSG << "socket for server has been created" << std::endl;

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    int ret = bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (ret < 0) {
        std::cout << SERV_MSG << "error binding conn" << std::endl;
        return -1;
    }

    listen(serverSocket, 5);
    std::map<int, std::string> clients;

    fd_set allFds, activeFds;
    FD_ZERO(&allFds);
    FD_SET(serverSocket, &allFds);
    int maxFd = serverSocket;

    while(true){
        activeFds = allFds;

        int sel = select(maxFd+1, &activeFds, nullptr, nullptr, nullptr);
        if(sel < 0){
            std::cout << SERV_MSG << "select error" << std::endl;
            break;
        }
        for(int fd=0; fd<=maxFd; fd++) {
            if(FD_ISSET(fd, &activeFds)) {
                if (fd == serverSocket) {
                    int clientSocket = accept(serverSocket, nullptr, nullptr);
                    if (clientSocket > 0) {
                        FD_SET(clientSocket, &allFds);
                        clients[clientSocket] = ""; // no name yet
                        if (clientSocket > maxFd)
                            maxFd = clientSocket;
                        std::cout << SERV_MSG << "client " << clientSocket << " connected, waiting for name" << std::endl;
                    }
                } else {
                    char buffer[1024] = {0};
                    ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);
                    if (bytes <= 0) {
                        std::cout << SERV_MSG << "client " << fd << "(" << clients[fd] << ") disconnected" << std::endl;
                        close(fd);
                        FD_CLR(fd, &allFds);
                        clients.erase(fd);
                    } else {
                        buffer[bytes] = '\0';
                        if (clients[fd].empty()) {
                            std::string name(buffer);
                            if (name.empty() || name.find_first_not_of(" \t\n\r") == std::string::npos) {
                                std::string msg = "Invalid name. Connection refused.\n";
                                send(fd, msg.c_str(), msg.size(), 0);
                                close(fd);
                                FD_CLR(fd, &allFds);
                                clients.erase(fd);
                            } else {
                                clients[fd] = name;
                                std::cout << SERV_MSG << "client " << fd << " set name: " << name << std::endl;
                            }
                            continue;
                        }
                        std::string msg = clients[fd] + ": " + buffer;
                        std::cout << msg << std::endl;
                        for (const auto& [clientFd, name] : clients)
                            if(clientFd != fd)
                                send(clientFd, msg.c_str(), msg.size(), 0);
                    }
                }
            }
        }
    }
    for (const auto& [client, name]:clients)
        close(client);

    close(serverSocket);
    return 0;
}