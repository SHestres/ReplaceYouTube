// ReplaceYouTube.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <net>
#include <string_view>

int main()
{
    std::cout << "Connecting to server..." << std::endl;

    std::net::io_context context;
    std::net::ip::tcp::resolver resolver(context);
    std::net::ip::tcp::socket socket(context);

    auto endpoints = resolver.resolve("localhost", "12345");
    std::net::connect(socket, endpoints);

    std::string message;
    message.resize(128);

    std::net::read(socket, std::net::buffer(message));

    std::cout << "Received message from server: " << message << std::endl;

    socket.shutdown(std::net::socket_base::shutdown_both);
    socket.close();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
