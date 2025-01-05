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

  EXPECT_EQ(request.method(), http::internals::HttpMethod::kGet);
}

TEST(HttpRequest, UnsupportedMethod) {
  const std::string request_str = "POST / HTTP/1.1\r\n";

  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.method(), http::internals::HttpMethod::kUnsupported);
}

TEST(HttpRequest, Path) {
  const std::string request_str = "GET /hello-world HTTP/1.1\r\n";

  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.path(), std::filesystem::path("/hello-world"));
}

TEST(HttpRequest, Version_1_1) {
  const std::string request_str = "GET / HTTP/1.1\r\n";

  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.version(), http::internals::HttpVersion::k1_1);
}
TEST(HttpRequest, UnsupportedVersion) {
  const std::string request_str = "GET / HTTP/1.0\r\n";

  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.version(), http::internals::HttpVersion::kUnsupported);
}

TEST(HttpRequest, HeaderFields) {
  const std::string safari_ua_string =
      "Mozilla/5.0 (iPhone; CPU iPhone OS 13_5_1 like Mac OS X) "
      "AppleWebKit/605.1.15 (KHTML, like Gecko) Version/13.1.1 Mobile/15E148 "
      "Safari/604.1";

  const std::string request_str =
      "GET / HTTP/1.1\r\n"
      "User-Agent: " +
      safari_ua_string + "\r\n";

  const http::internals::Request request(
      std::vector<char>(request_str.cbegin(), request_str.cend()));

  EXPECT_EQ(request.header_fields().Size(), 1);
  EXPECT_EQ(request.header_fields()["user-agent"], safari_ua_string);
}
