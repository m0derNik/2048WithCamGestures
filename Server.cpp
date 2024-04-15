#include "Board.hpp"
#include "Screen.hpp"
#include "Other.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>// Для работы с потоками

#pragma comment (lib, "ws2_32.lib")

void RunAI() {
    system("python cppHand.py");
}

void start_server(Board& board, RenderWindow& window, Screen& screen) {

    // Initialize Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
        return;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        WSACleanup();
        return;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(65432);
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    std::thread t(RunAI);
    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char buf[4096];
    while (true) {
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }

        if (bytesReceived == 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }

        // Display swipe in console
        std::string message = std::string(buf, 0, bytesReceived);
        if (message == "Swipe Up") {
            make_move(board, 0);
            board.new_step();
            message = "";
        }
        else if (message == "Swipe Down") {
            make_move(board, 1);
            board.new_step();
            message = "";
        }
        else if (message == "Swipe Left") {
            make_move(board, 2);
            board.new_step();
            message = "";
        }
        else if (message == "Swipe Right") {
            make_move(board, 3);
            board.new_step();
            message = "";
        }
        window.clear(Color(187, 173, 160));
        screen.set_screen(board.tiles, board.score);
        screen.display(&window);

    }

    // Close socket
    closesocket(clientSocket);

    // Shutdown Winsock
    WSACleanup();

    t.join();
}


//#include "Board.hpp"
//#include "Screen.hpp"
//#include "Other.hpp"
//#include "Server.hpp"
//#include <iostream>
//#include <string>
//#include <WS2tcpip.h>
//#include <thread> // Для работы с потоками
//
//#pragma comment (lib, "ws2_32.lib")
//
// Функция для обработки запросов клиента
//void handle_client(SOCKET clientSocket, Board& board) {
//    char buf[4096];
//    while (true) {
//        ZeroMemory(buf, 4096);
//
//         Wait for client to send data
//        int bytesReceived = recv(clientSocket, buf, 4096, 0);
//        if (bytesReceived == SOCKET_ERROR) {
//            std::cerr << "Error in recv(). Quitting" << std::endl;
//            break;
//        }
//
//        if (bytesReceived == 0) {
//            std::cout << "Client disconnected" << std::endl;
//            break;
//        }
//
//         Display swipe in console
//        std::string message = std::string(buf, 0, bytesReceived);
//        if (message == "Swipe Up") {
//            make_move(board, 0);
//            board.new_step();
//            message = "";
//        }
//        else if (message == "Swipe Down") {
//            make_move(board, 1);
//            board.new_step();
//            message = "";
//        }
//        else if (message == "Swipe Left") {
//            make_move(board, 2);
//            board.new_step();
//            message = "";
//        }
//        else if (message == "Swipe Right") {
//            make_move(board, 3);
//            board.new_step();
//            message = "";
//        }
//    }
//
//     Close socket
//    closesocket(clientSocket);
//}
//
//void start_server(Board& board) {
//     Initialize Winsock
//    WSADATA wsData;
//    WORD ver = MAKEWORD(2, 2);
//    int wsOk = WSAStartup(ver, &wsData);
//    if (wsOk != 0) {
//        std::cerr << "Can't initialize Winsock! Quitting" << std::endl;
//        return;
//    }
//
//     Create a socket
//    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
//    if (listening == INVALID_SOCKET) {
//        std::cerr << "Can't create a socket! Quitting" << std::endl;
//        WSACleanup();
//        return;
//    }
//
//     Bind the ip address and port to a socket
//    sockaddr_in hint;
//    hint.sin_family = AF_INET;
//    hint.sin_port = htons(65432);
//    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
//
//    bind(listening, (sockaddr*)&hint, sizeof(hint));
//
//     Tell Winsock the socket is for listening
//    listen(listening, SOMAXCONN);
//
//     Wait for a connection
//    sockaddr_in client;
//    int clientSize = sizeof(client);
//
//     Main loop to accept incoming connections
//    while (true) {
//        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
//
//         Handle the client connection in a separate thread
//        std::thread clientThread(handle_client, clientSocket, std::ref(board));
//        clientThread.detach(); // Detach the thread to allow it to run independently
//    }
//
//     Close listening socket
//    closesocket(listening);
//
//     Shutdown Winsock
//    WSACleanup();
//}
