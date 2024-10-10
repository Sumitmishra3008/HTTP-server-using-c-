#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")  // Link with Winsock library

const int PORT = 8080;

int main() {
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return -1;
    }
    

    // Create a TCP socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error creating socket." << std::endl;
        WSACleanup();
        return -1;
    }

    // Setup the address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket to address." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        std::cerr << "Error listening for connections." << std::endl;
        closesocket(server_socket);
        WSACleanup();
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Server loop: accept and handle incoming connections
    while (true) {
        // Accept a client connection
        if ((client_socket = accept(server_socket, (struct sockaddr*)&address, &addrlen)) == INVALID_SOCKET) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        // Buffer to store incoming data
        char buffer[1024] = {0};

        // Read the client's request
        int bytes_read = recv(client_socket, buffer, 1023, 0);
        if (bytes_read > 0) {
            // Print the request to the console
            std::string request(buffer);
            std::cout << "Request:\n" << request << std::endl;

            // Create a simple HTTP response
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello from the C++ server!";
            
            // Send the response to the client
            send(client_socket, response.c_str(), response.size(), 0);
        }

        // Close the client socket
        closesocket(client_socket);
    }

    // Cleanup Winsock before exiting (this line will not be reached in this infinite loop)
    closesocket(server_socket);
    WSACleanup();
    return 0;
}
