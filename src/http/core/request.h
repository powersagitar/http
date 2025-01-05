// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef REQUEST_H
#define REQUEST_H

#include <filesystem>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace http::internals {
class Method {
 public:
  enum MethodEnum { kGet, kUnsupported };

  Method() noexcept : method_(kUnsupported) {}

  explicit Method(const std::string_view str) noexcept {
    std::unordered_map<std::string_view, MethodEnum> map{{"GET", kGet}};

    method_ = map.contains(str) ? map[str] : kUnsupported;
  }

  bool operator==(const MethodEnum method) const noexcept {
    return method_ == method;
  }

  [[nodiscard]] std::string_view ToString() const noexcept {
    std::unordered_map<MethodEnum, std::string_view> map{
        {kGet, "GET"}, {kUnsupported, "UNSUPPORTED"}};

    return map[method_];
  }

 private:
  MethodEnum method_;
};

class Request {
 public:
  explicit Request(const std::vector<char> &buffer) noexcept;

  [[nodiscard]] Method method() const noexcept { return method_; }
  [[nodiscard]] std::filesystem::path path() const noexcept { return path_; }

 private:
  Method method_;
  std::filesystem::path path_;
};
}  // namespace http::internals

#endif  // REQUEST_H
