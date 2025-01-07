// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include "response.h"

#include <filesystem>
#include <string>
#include <unordered_map>

namespace http::internals {
std::unique_ptr<Response> ConstructResponse(
    std::filesystem::path path) noexcept {
  if (!std::filesystem::exists(path)) {
    return std::make_unique<ResponseNotFound>();
  }

  if (std::filesystem::is_directory(path)) {
    path /= "index.html";
  }

  const std::unordered_map<std::string, std::unique_ptr<Response> (*)()>
      ext_to_response{{".html", []() -> std::unique_ptr<Response> {
                         return std::make_unique<ResponseHtml>();
                       }}};

  if (!ext_to_response.contains(path.extension())) {
    // TODO: return unsupported
  }

  return ext_to_response.at(path.extension())();
}
}  // namespace http::internals