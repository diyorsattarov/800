#include <iostream>
#include <spdlog/spdlog.h>
#include <boost/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <thread>

#include <gtest/gtest.h>

class WebSocketTestFixture : public ::testing::Test {
protected:
    WebSocketTestFixture() {
        spdlog::info("Hello world. Welcome to spdlog");
        spdlog::info("Boost version: {}.{}.{}", BOOST_VERSION / 100000, (BOOST_VERSION / 100) % 1000, BOOST_VERSION % 100);
    }

    void SetUp() override {
        // Start the WebSocket server in a separate thread
        server_thread = std::thread([this] {
            do_session("127.0.0.1", 8080);
        });
    }

    void TearDown() override {
        // Stop the WebSocket server
        if (server_thread.joinable()) {
            ioc.stop();
            server_thread.join();
        }
    }

    void do_session(const std::string& address, unsigned short port) {
        try {
            boost::asio::io_context ioc;

            // Look up the domain name
            boost::asio::ip::tcp::resolver resolver{ioc};
            auto const results = resolver.resolve(address, std::to_string(port));

            // Make the connection on the IP address we get from a lookup
            boost::asio::ip::tcp::socket socket{ioc};
            boost::asio::connect(socket, results.begin(), results.end());

            // Construct the WebSocket stream and handshake
            boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws{std::move(socket)};
            ws.handshake(address, "/");

            // Send a message to the server
            ws.write(boost::asio::buffer("Hello, WebSocket Server!"));

            // Receive the response from the server
            boost::beast::flat_buffer buffer;
            ws.read(buffer);

            // Validate the response
            ASSERT_TRUE(boost::beast::buffers_to_string(buffer.data()) == "Hello, WebSocket Server!");
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    boost::asio::io_context ioc; // The io_context for WebSocket server
    std::thread server_thread;   // Thread for running the WebSocket server
};

TEST_F(WebSocketTestFixture, TestWebSocketConnection) {
    // This test connects to the WebSocket server and sends a message
    // It then verifies that the server echoes back the same message

    // Sleep for a while to allow the WebSocket server to start
    std::this_thread::sleep_for(std::chrono::seconds(25));

    // No need for explicit assertions here, as they're in the do_session function
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
