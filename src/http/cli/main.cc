// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <http/core/http.h>
#include <sys/socket.h>

#include <cstdint>

#include "spdlog/spdlog.h"

[[noreturn]] int main() {
  constexpr size_t kBufferSize = 1024;
  constexpr uint16_t kPortHost = 80;

  spdlog::set_level(spdlog::level::trace);

  http::Start({.buffer_size = kBufferSize,
               .port = htons(kPortHost),
               .cwd = "/tmp/http-cwd"});
}