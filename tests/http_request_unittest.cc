// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <gtest/gtest.h>
#include <http/core/request.h>

#include <filesystem>

TEST(HttpRequest, Get) {
  const std::string request_str = "GET / HTTP/1.1\r\n";

  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.method(), http::internals::Method::kGet);
  EXPECT_EQ(request.path(), std::filesystem::path("/"));
  EXPECT_EQ(request.version(), http::internals::HttpVersion::k1_1);
}

TEST(HttpRequest, UnsupportedMethod) {
  const std::string request_str = "POST / HTTP/1.1\r\n";

  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.method(), http::internals::Method::kUnsupported);
  EXPECT_EQ(request.path(), std::filesystem::path("/"));
  EXPECT_EQ(request.version(), http::internals::HttpVersion::k1_1);
}
