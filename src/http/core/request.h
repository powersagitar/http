// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#ifndef REQUEST_H
#define REQUEST_H

#include <algorithm>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace http::internals {
class Method {
 public:
  enum _Method { kGet, kUnsupported };

  Method() noexcept : method_(kUnsupported) {}

  Method(std::string str) noexcept {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](char c) { return std::tolower(c); });

    std::unordered_map<std::string_view, _Method> map{{"get", _Method::kGet}};

    method_ = map.contains(str) ? map[str] : kUnsupported;
  }

  bool operator==(const _Method method) const noexcept {
    return method_ == method;
  }

  std::string_view ToString() const noexcept {
    std::unordered_map<_Method, std::string_view> map{
        {_Method::kGet, "GET"}, {kUnsupported, "UNSUPPORTED"}};

    return map[method_];
  }

 private:
  _Method method_;
};

class Request {
 public:
  explicit Request(const std::vector<char> &buffer) noexcept;

  Method method() const noexcept { return method_; }
  std::filesystem::path path() const noexcept { return path_; }

 private:
  Method method_;
  std::filesystem::path path_;
};
}  // namespace http::internals

#endif  // REQUEST_H
