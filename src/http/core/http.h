// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef HTTP_H
#define HTTP_H

// Required by in_port_t
// ReSharper disable once CppUnusedIncludeDirective
#include <netinet/in.h>

#include <cstddef>
#include <filesystem>

namespace http {
struct Config {
  size_t buffer_size;
  in_port_t port;
  std::filesystem::path cwd;
};

[[noreturn]] void Start(const Config &config);
}  // namespace http

#endif  // HTTP_H
