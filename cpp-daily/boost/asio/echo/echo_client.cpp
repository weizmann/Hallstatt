//
// Created by 郑伟 on 2018/5/1.
//

#include <boost/asio.hpp>

#include <iostream>

using boost::asio::ip::tcp;

enum {
    BUFFER_SIZE = 1024,
};

void session(tcp::socket socket) {
    try {
        while (true) {
            boost::array<char, BUFFER_SIZE> data;
            boost::system::error_code ec;
            std::size_t length = socket.read_some(boost::asio::buffer(data), ec);
            if (ec == boost::asio::error::eof) {

            } else if (ec) {
                std::cerr << ec << std::endl;
            } else {
                boost::asio::write(socket, )
            }

        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}