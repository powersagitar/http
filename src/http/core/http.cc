// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "http.h"

#include <cstdlib>
#include <vector>

#include "request.h"
#include "response.h"
#include "spdlog/spdlog.h"
#include "tcp.h"

namespace http {
void Start(const Config &config) {
  const http::TcpServer server;

  spdlog::info("Binding server socket...");
  spdlog::debug("sin_family: AF_INET");
  spdlog::debug("sin_port: {}", config.port);
  spdlog::debug("sin_addr: INADDR_ANY");

  if (server.Bind(AF_INET, htons(config.port), {INADDR_ANY}) == 0) {
    spdlog::info("Server socket bound successfully.");
  } else {
    spdlog::critical("Server socket failed to bind.");
    std::exit(EXIT_FAILURE);
  }

  spdlog::info("Listening on server socket...");
  spdlog::debug("backlog: 512");

  if (server.Listen() == 0) {
    spdlog::info("Listening on server socket.");
  } else {
    spdlog::critical("Failed to listen on server socket.");
    std::exit(EXIT_FAILURE);
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

    if (bytes_read <= 0) {
      spdlog::error("Failed to read request from client.");
    }

    const internals::Request request(buffer);

    spdlog::info("Request method: {}", request.method().ToString());
    spdlog::info("Request path: {}", request.path().string());
    spdlog::info("Request http version: {}", request.version().ToString());
    spdlog::info("Request header fields: \n{}",
                 request.header_fields().ToString());

    const std::vector<std::byte> &response_bytes =
        internals::ConstructResponseBytes(request.path(), config.cwd);

    const ssize_t bytes_sent =
        client.Send(response_bytes.data(), response_bytes.size());

    if (bytes_sent <= 0) {
      spdlog::error("Failed to send response to client.");
    }
  }
  // ReSharper disable once CppDFAUnreachableCode
}
}  // namespace http