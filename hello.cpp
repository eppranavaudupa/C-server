#include <iostream>
#include <cstring>      // For memset
#include <unistd.h>    // For close
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Bind the socket to an address and port
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080); // Port number

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Error binding socket\n";
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Error listening\n";
        return 1;
    }

    // Accept incoming connections
    int new_socket;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        std::cerr << "Error accepting connection\n";
        return 1;
    }

    // Send data to the client
    const char *message = "Hello from server!";
    send(new_socket, message, strlen(message), 0);

    // Close the socket
    close(server_fd);

    return 0;
}

