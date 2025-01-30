#include <asio.hpp>
#include "include/WebServer.h"

using namespace std;


int main() {
    asio::io_context event_loop(1);

    WebServer webServer;
    co_spawn(event_loop, webServer.start(), asio::detached);

    event_loop.run();
}
