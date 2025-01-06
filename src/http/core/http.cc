// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

//
// Created by powersagitar on 1/3/25.
//

#include "http.h"

#include <string>
#include <vector>

#include "request.h"
#include "spdlog/spdlog.h"
#include "tcp.h"

namespace http {
void Start(const Config config) {
  const http::TcpServer server;

  spdlog::info("Binding server socket...");
  spdlog::debug("sin_family: AF_INET");
  spdlog::debug("sin_port: {}", config.port);
  spdlog::debug("sin_addr: INADDR_ANY");

  if (server.Bind(AF_INET, config.port, {INADDR_ANY}) == 0) {
    spdlog::info("Server socket bound successfully.");
  } else {
    spdlog::critical("Server socket failed to bind.");
  }

  spdlog::info("Listening on server socket...");
  spdlog::debug("backlog: 512");

  if (server.Listen() == 0) {
    spdlog::info("Listening on server socket.");
  } else {
    spdlog::critical("Failed to listen on server socket.");
  }

  while (true) {
    spdlog::info("Waiting for client connection...");

    const TcpClient client = [&server] {
      try {
        return server.Accept();
      } catch (const std::system_error &e) {
        spdlog::error("Failed to accept client socket: {}", e.what());
        return TcpClient(internals::kNullSock, {}, 0);
      }
    }();

    if (client.sockfd() == internals::kNullSock) {
      spdlog::info("Skipping client connection.");
      continue;
    }

    spdlog::info("Accepted client connection.");

    std::vector<char> buffer(config.buffer_size);

    const ssize_t bytes_read =
        client.Recv(buffer.data(), static_cast<ssize_t>(buffer.size()));

    spdlog::info("Received {} bytes from client.", bytes_read);

    const internals::Request request(buffer);

    spdlog::info("Request method: {}", request.method().ToString());
    spdlog::info("Request path: {}", request.path().string());
    spdlog::info("Request http version: {}", request.version().ToString());
    spdlog::info("Request header fields: \n{}",
                 request.header_fields().ToString());

    const std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 13\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello, World!";

    const ssize_t bytes_sent = client.Send(response.c_str(), response.length());

    spdlog::info("Sent {} bytes to client.", bytes_sent);
  }
  // ReSharper disable once CppDFAUnreachableCode
}
}  // namespace http