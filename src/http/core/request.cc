// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

//
// Created by powersagitar on 1/4/25.
//

#include "request.h"

#include <sstream>
#include <string>
#include <vector>

namespace http::internals {
Request::Request(const std::vector<char> &buffer) noexcept {
  const std::string buffer_str(buffer.cbegin(), buffer.cend());
  std::istringstream buffer_stream(buffer_str);

  std::string method_str;
  buffer_stream >> method_str;

  method_ = Method(method_str);

  std::string path_str;
  buffer_stream >> path_str;

  path_ = std::filesystem::path(path_str);

  std::string version_str;
  buffer_stream >> version_str;

  version_ = HttpVersion(version_str);
}
}  // namespace http::internals