// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef HTTP_H
#define HTTP_H

#include <netinet/in.h>

#include <cstddef>

namespace http {
struct Config {
  size_t buffer_size;
  in_port_t port;
};

[[noreturn]] void Start(Config config);
}  // namespace http

#endif  // HTTP_H
