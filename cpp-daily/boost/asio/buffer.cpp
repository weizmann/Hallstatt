#include <boost/asio.hpp>
#include <iostream>

void Print(boost::system::error_code ec) {
    std::cout << "Hello, world" << std::endl;
}

int main() {
    boost::asio::io_context io_context;
    boost::asio::deadline_timer timer(io_context, boost::posix_time::seconds(3));

    timer.async_wait(&Print);

    io_context.run();

    return 0;
}