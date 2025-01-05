// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

//
// Created by powersagitar on 1/4/25.
//

#include "request.h"

#include <boost/algorithm/string.hpp>
#include <sstream>
#include <string>
#include <vector>

namespace http::internals {
HttpHeaderFields::HttpHeaderFields(
    std::istringstream &buffer_istream) noexcept {
  for (std::string line; std::getline(buffer_istream, line);) {
    const size_t colon_idx = line.find(':');
    std::string key = line.substr(0, colon_idx);
    std::string value = line.substr(colon_idx + 1);

    boost::algorithm::to_lower(key);
    boost::algorithm::trim(value);

    fields_[key] = value;
  }
}

Request::Request(const std::vector<char> &buffer) noexcept {
  const std::string buffer_str(buffer.cbegin(), buffer.cend());
  std::istringstream buffer_istream(buffer_str);

  std::string method_str;
  buffer_istream >> method_str;

  method_ = HttpMethod(method_str);

  std::string path_str;
  buffer_istream >> path_str;

  path_ = std::move(std::filesystem::path(path_str));

  std::string version_str;
  buffer_istream >> version_str;

  version_ = HttpVersion(version_str);

  // Ignore the rest of the first line
  buffer_istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  header_fields_ = std::move(HttpHeaderFields(buffer_istream));
}
}  // namespace http::internals