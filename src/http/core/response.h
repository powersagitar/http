// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef RESPONSE_H
#define RESPONSE_H

#include <cstddef>
#include <filesystem>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

namespace http::internals {
class StatusCode {
 public:
  enum Value : unsigned short {
    kOK = 200,
    kNotFound = 404,
    kNotImplemented = 501,
  };

  explicit StatusCode(const Value value) noexcept : value_(value) {}

  constexpr unsigned short ToUnsignedShort() const noexcept { return value_; }

  std::string ToString() const noexcept {
    std::unordered_map<Value, std::string> value_to_string{
        {kOK, "OK"},
        {kNotFound, "Not Found"},
        {kNotImplemented, "Not Implemented"}};

    return value_to_string.at(value_);
  }

 private:
  Value value_ = kNotImplemented;
};

class Response {
 public:
  virtual ~Response() = default;

  virtual StatusCode GetStatusCode() const noexcept = 0;

  virtual std::string GetMimeType() const noexcept = 0;

  virtual std::vector<std::byte> GetBody() const noexcept = 0;
};

class ResponseNotFound final : public Response {
 public:
  ~ResponseNotFound() override = default;

  StatusCode GetStatusCode() const noexcept override {
    return StatusCode(StatusCode::kNotFound);
  }

  std::string GetMimeType() const noexcept override { return "text/plain"; }

  std::vector<std::byte> GetBody() const noexcept override {
    const std::string body = "404 Not Found";
    std::vector<std::byte> body_bytes(body.length());

    std::ranges::transform(
        body.cbegin(), body.cend(), body_bytes.begin(),
        [](const char character) { return static_cast<std::byte>(character); });

    return body_bytes;
  }
};

class ResponseHtml final : public Response {
 public:
  ~ResponseHtml() override = default;

  StatusCode GetStatusCode() const noexcept override {
    return StatusCode(StatusCode::kOK);
  }

  std::string GetMimeType() const noexcept override { return "text/plain"; }

  std::vector<std::byte> GetBody() const noexcept override {
    const std::string body = "404 Not Found";
    std::vector<std::byte> body_bytes(body.length());

    std::ranges::transform(body, body_bytes.begin(), [](const char character) {
      return static_cast<std::byte>(character);
    });

    return body_bytes;
  }
};

std::unique_ptr<Response> ConstructResponseObject(
    std::filesystem::path path) noexcept;

std::string ConstructResponseHeader(const Response& response) noexcept;

std::vector<std::byte> ConstructResponseBytes(
    const std::filesystem::path& request_path,
    const std::filesystem::path& cwd) noexcept;
}  // namespace http::internals

#endif  // RESPONSE_H
