// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef RESPONSE_H
#define RESPONSE_H

#include <__filesystem/filesystem_error.h>

#include <cstddef>
#include <string>
#include <vector>

namespace http::internals {
enum class StatusCode : unsigned short {
  kOK = 200,
  kNotImplemented = 501,
};

class Response {
 public:
  virtual ~Response() = default;

  virtual std::vector<std::byte> GetBody() const noexcept = 0;

  virtual std::string GetMimeType() const noexcept = 0;
};

class ResponseNotFound : public Response {
 public:
  ~ResponseNotFound() override = default;

  std::vector<std::byte> GetBody() const noexcept override {
    const std::string body = "404 Not Found";
    std::vector<std::byte> body_bytes(body.length());

    std::ranges::transform(
        body.cbegin(), body.cend(), body_bytes.begin(),
        [](const char character) { return static_cast<std::byte>(character); });

    return body_bytes;
  }

  std::string GetMimeType() const noexcept override { return "text/plain"; }
};

class ResponseHtml : public Response {
 public:
  ~ResponseHtml() override = default;

  std::vector<std::byte> GetBody() const noexcept override {
    const std::string body = "404 Not Found";
    std::vector<std::byte> body_bytes(body.length());

    std::ranges::transform(
        body.cbegin(), body.cend(), body_bytes.begin(),
        [](const char character) { return static_cast<std::byte>(character); });

    return body_bytes;
  }

  std::string GetMimeType() const noexcept override { return "text/plain"; }
};

std::unique_ptr<Response> ConstructResponse(
    std::filesystem::path path) noexcept;
}  // namespace http::internals

#endif  // RESPONSE_H
