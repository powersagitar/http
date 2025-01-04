// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <gtest/gtest.h>
#include <http/core/request.h>

#include <filesystem>

namespace {
std::string request_str = "GET / HTTP/1.1\r\n";
}

TEST(HttpRequest, Method) {
  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.method(), http::internals::Method::kGet);
  EXPECT_EQ(request.path(), std::filesystem::path("/"));
}