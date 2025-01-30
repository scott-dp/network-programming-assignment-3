#include <iostream>
#include <asio.hpp>

using namespace std;

class WebServer {
    asio::awaitable<void> handle_request(asio::ip::tcp::socket socket) {
        for (;;) {
            std::string buffer;
            auto bytes_transferred = co_await asio::async_read_until(socket, asio::dynamic_buffer(buffer), "\r\n", asio::use_awaitable);
            auto message = buffer.substr(0, bytes_transferred - 2); // Strip \r\n at end of buffer
            cout << "Server: received: " << message << endl;
            // Close socket when "exit" is retrieved from client
            if (message == "exit") {
                cout << "Server: closing connection" << endl;
                // Connection is closed when socket is destroyed
                co_return;
            }
            bytes_transferred = co_await asio::async_write(socket, asio::buffer(buffer.substr(0, bytes_transferred-3) + "\r\n"), asio::use_awaitable);
            message = buffer.substr(0, bytes_transferred-2);
            cout << "Server: sent: " << message << endl;
        }
    }

public:
    WebServer() {}

    asio::awaitable<void> start() {
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::acceptor acceptor(executor, {asio::ip::tcp::v6(), 3000});

        cout << "Server: waiting for connection" << endl;
        for (;;) {
            auto socket = co_await acceptor.async_accept(asio::use_awaitable);
            cout << "Server: connection from " << socket.remote_endpoint().address() << ':' << socket.remote_endpoint().port() << endl;
            co_spawn(executor, handle_request(std::move(socket)), asio::detached);
        }
    }
};

class WebClient {

public:
    asio::awaitable<void> start() {
        auto executor = co_await asio::this_coro::executor;
        asio::ip::tcp::resolver resolver(executor);

        // Resolve query (DNS-lookup if needed)
        auto results = co_await resolver.async_resolve("localhost", to_string(3000), asio::use_awaitable);

        asio::ip::tcp::socket socket(executor);
        co_await asio::async_connect(socket, results, asio::use_awaitable);
        cout << "Client: connected" << endl;

        std::string message("hello");
        auto bytes_transferred = co_await asio::async_write(socket, asio::buffer(message + "\r\n"), asio::use_awaitable);
        cout << "Client: sent: " << message << endl;

        std::string buffer;
        bytes_transferred = co_await asio::async_read_until(socket, asio::dynamic_buffer(buffer), "\r\n", asio::use_awaitable);
        message = buffer.substr(0, bytes_transferred - 2); // Strip \r\n at end of buffer
        cout << "Client: received: " << message << endl;

        message = "exit";
        bytes_transferred = co_await asio::async_write(socket, asio::buffer(message + "\r\n"), asio::use_awaitable);
        cout << "Client: sent: " << message << endl;
    }
};

int main() {
    asio::io_context event_loop(1);

    WebServer echo_server;
    co_spawn(event_loop, echo_server.start(), asio::detached);

    WebClient echo_client;
    co_spawn(event_loop, echo_client.start(), asio::detached);

    event_loop.run();
}
