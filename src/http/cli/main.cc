// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <http/core/socket.h>

#include <array>
#include <iostream>

int main() {
  const http::SocketServer server;
  server.Bind();
  server.Listen(1);

  const http::SocketClient client = server.Accept();

  constexpr size_t kBufSize = 1024;

  std::array<char, kBufSize> buf{'\0'};

  const ssize_t bytes_read =
      client.Read(static_cast<void *>(buf.data()), kBufSize - 1);
  buf.at(bytes_read) = '\0';

  std::cout << "Server received: " << buf.data() << "\n";

  return 0;
}