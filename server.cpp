#include <sys/socket.h> // for socket functions
#include <netinet/in.h> // for sockaddr_in
#include <cstdlib> // for exit() and EXIT_FAILURE
#include <iostream> // for cout
#include <unistd.h> // for read

int main() {
    // create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cout << "Failed to create socket!  errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    // htons is necessary to convert a number to network byte order
    sockaddr.sin_port = htons(9999); 

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        std::cout << "Failed to bind to port 9999!  errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // start listening, hold at most 10 connections in the backlog
    if (listen(sockfd, 10) <0) {
        std::cout << "Failed to listen on socket!  errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // grab a connection from the queue
    auto addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
        std::cout << "Failed to grab connection!  errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // read from the connection
    char buffer[100];
    auto bytesRead = read(connection, buffer, 100);
    std::cout << "The message was: " << buffer << "!" <<std::endl;

    // send a message to the connection
    std::string response = "Nice chat... I guess\n";
    send(connection, response.c_str(), response.size(), 0);

    // close the connections
    close(connection);
    close(sockfd);
}
