// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef REQUEST_H
#define REQUEST_H

#include <filesystem>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace http::internals {
class HttpMethod {
 public:
  enum MethodEnum { kGet, kUnsupported };

  HttpMethod() noexcept : method_(kUnsupported) {}

  explicit HttpMethod(const std::string_view str) noexcept {
    std::unordered_map<std::string_view, MethodEnum> map{{"GET", kGet}};

    method_ = map.contains(str) ? map[str] : kUnsupported;
  }

  [[nodiscard]] bool operator==(const MethodEnum method_rhs) const noexcept {
    return method_ == method_rhs;
  }

  [[nodiscard]] std::string_view ToString() const noexcept {
    std::unordered_map<MethodEnum, std::string_view> map{
        {kGet, "GET"}, {kUnsupported, "UNSUPPORTED"}};

    return map[method_];
  }

 private:
  MethodEnum method_;
};

class HttpVersion {
 public:
  enum VersionEnum { k1_1, kUnsupported };

  HttpVersion() noexcept : version_(kUnsupported) {}

  explicit HttpVersion(const std::string_view str) noexcept {
    std::unordered_map<std::string_view, VersionEnum> map{{"HTTP/1.1", k1_1}};

    version_ = map.contains(str) ? map[str] : kUnsupported;
  }

  [[nodiscard]] bool operator==(const VersionEnum version_rhs) const noexcept {
    return version_ == version_rhs;
  }

  [[nodiscard]] std::string_view ToString() const noexcept {
    std::unordered_map<VersionEnum, std::string_view> map{
        {k1_1, "HTTP/1.1"}, {kUnsupported, "UNSUPPORTED"}};

    return map[version_];
  }

 private:
  VersionEnum version_;
};

class HttpHeaderFields {
 public:
  HttpHeaderFields() = default;

  explicit HttpHeaderFields(std::istringstream &buffer_istream) noexcept;

  [[nodiscard]] bool Contains(const std::string &key) const noexcept {
    return fields_.contains(key);
  }

  [[nodiscard]] std::string_view operator[](const std::string &key) const {
    return fields_.at(key);
  }

  [[nodiscard]] size_t Size() const noexcept { return fields_.size(); }

  [[nodiscard]] std::string ToString() const noexcept;

 private:
  std::unordered_map<std::string, std::string> fields_;
};

class Request {
 public:
  explicit Request(const std::vector<char> &buffer) noexcept;

  [[nodiscard]] HttpMethod method() const noexcept { return method_; }

  [[nodiscard]] std::filesystem::path path() const noexcept { return path_; }

  [[nodiscard]] HttpVersion version() const noexcept { return version_; };

  [[nodiscard]] const HttpHeaderFields &header_fields() const noexcept {
    return header_fields_;
  }

 private:
  HttpMethod method_;
  std::filesystem::path path_;
  HttpVersion version_;
  HttpHeaderFields header_fields_;
};
}  // namespace http::internals

#endif  // REQUEST_H
