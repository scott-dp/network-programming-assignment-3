//
// Created by scott on 30.01.2025.
//
#include <iostream>
#include <asio.hpp>

#ifndef TCP_SERVER_WEBSERVER_H
#define TCP_SERVER_WEBSERVER_H


class WebServer {
public:
    asio::awaitable<void> handle_request(asio::ip::tcp::socket socket);
    asio::awaitable<void> start();
    WebServer();
};


#endif //TCP_SERVER_WEBSERVER_H
