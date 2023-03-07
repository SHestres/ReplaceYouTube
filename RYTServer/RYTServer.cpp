// RYTServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <net>
#include <string_view>


int main()
{
    std::cout << "Starting server..." << std::endl;

    std::net::io_context context;
    std::net::ip::tcp::acceptor acceptor(context, { std::net::ip::tcp::v4(), 12345 });

    while (true)
    {
        std::net::ip::tcp::socket socket(context);
        acceptor.accept(socket);

        std::cout << "Client connected: " << socket.remote_endpoint() << std::endl;

        std::string message = "Hello from server!";
        std::net::write(socket, std::net::buffer(message));

        socket.shutdown(std::net::socket_base::shutdown_send);
    }

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
