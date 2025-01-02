//
// Created by powersagitar on 1/2/25.
//

#include <gtest/gtest.h>
#include <http/http.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <array>
#include <cstring>
#include <future>
#include <string>

namespace {

constexpr int kBufSize = 1024;
constexpr std::string server_expects_to_receive = "Hello, World!";

void Server() {
  http::SocketServer server;
  ASSERT_EQ(server.Bind(), 0);
  ASSERT_EQ(server.Listen(), 0);

  const http::SocketClient accepted_client = [&server] {
    try {
      return server.Accept();
    } catch (const std::system_error &e) {
      [&e] { FAIL() << e.what(); }();
      return http::SocketClient(0, {}, 0);
    }
  }();

  std::array<char, kBufSize> server_received;

  const ssize_t bytes_read =
      accepted_client.Read(server_received.data(), kBufSize - 1);

  server_received.at(bytes_read) = '\0';

  ASSERT_EQ(
      std::strcmp(server_received.data(), server_expects_to_receive.c_str()),
      0);
}

void Client() {
  const int client = socket(PF_INET, SOCK_STREAM, 0);
  ASSERT_GT(client, -1);

  sockaddr_in addr{.sin_family = AF_INET, .sin_port = 80, .sin_zero{0}};
  ASSERT_EQ(inet_aton("127.0.0.1", &addr.sin_addr), 1);

  ASSERT_EQ(connect(client, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)),
            0);

  send(client, server_expects_to_receive.c_str(),
       server_expects_to_receive.length(), 0);
}

}  // namespace

TEST(Socket, AcceptIncomingConnections) {
  auto server_future = std::async(std::launch::async, Server);
  auto client_future = std::async(std::launch::async, Client);

  server_future.get();
  client_future.get();
}