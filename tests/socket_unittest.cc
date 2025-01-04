// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <gtest/gtest.h>
#include <http/core/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <array>
#include <cstring>
#include <future>
#include <string>

namespace {

constexpr int kBufSize = 1024;
constexpr int kHTTPPort = 80;
constexpr std::string server_expects_to_receive = "Hello, World!";

void Server() {
  http::TcpServer server;
  ASSERT_EQ(server.Bind(), 0);
  ASSERT_EQ(server.Listen(), 0);

  const http::TcpClient accepted_client = [&server] {
    try {
      return server.Accept();
    } catch (const std::system_error &e) {
      [&e] { FAIL() << e.what(); }();
      return http::TcpClient(0, {}, 0);
    }
  }();

  std::array<char, kBufSize> server_received{};

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

  sockaddr_in addr{.sin_family = AF_INET, .sin_port = kHTTPPort, .sin_zero{0}};
  ASSERT_EQ(inet_aton("127.0.0.1", &addr.sin_addr), 1);

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  ASSERT_EQ(connect(client, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)),
            0);

  send(client, server_expects_to_receive.c_str(),
       server_expects_to_receive.length(), 0);
}

}  // namespace

TEST(Socket, ReadWrite) {
  auto server_future = std::async(std::launch::async, Server);
  auto client_future = std::async(std::launch::async, Client);

  server_future.get();
  client_future.get();
}