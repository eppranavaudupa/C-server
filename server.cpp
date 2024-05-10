#include <iostream>
#include <string>
#include <boost/beast.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;

int main() {
    try {
        // Create the I/O context
        net::io_context ioc;

        // Create an endpoint and acceptor
        net::ip::tcp::endpoint endpoint(net::ip::make_address("0.0.0.0"), 3000);
        net::ip::tcp::acceptor acceptor(ioc, endpoint);

        while (true) {
            // Wait for incoming connection
            net::ip::tcp::socket socket(ioc);
            acceptor.accept(socket);

            // Read request
            beast::flat_buffer buffer;
            http::request<http::string_body> request;
            http::read(socket, buffer, request);

            // Send response
            http::response<http::string_body> response{http::status::ok, request.version()};
            response.set(http::field::server, "C++ HTTP Server");
            response.set(http::field::content_type, "text/html");
            response.keep_alive(request.keep_alive());
            response.body() = "hello from the server";
            response.prepare_payload();
            http::write(socket, response);
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
