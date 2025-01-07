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
std::unique_ptr<Response> ConstructResponseObject(
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

std::string ConstructResponseHeader(const Response& response) noexcept {
  const std::string http_version = "HTTP/1.1";
  const StatusCode status_code = response.GetStatusCode();
  const std::string& mime_type = response.GetMimeType();
  const std::vector<std::byte>& body = response.GetBody();

  std::string header;

  header += http_version + " ";
  header += std::to_string(status_code.ToUnsignedShort()) + " ";
  header += status_code.ToString() + "\r\n";

  header += "Content-Length: " + std::to_string(body.size()) + "\r\n";
  header += "Content-Type: " + mime_type + "\r\n";

  return header;
}

std::vector<std::byte> ConstructResponseBytes(
    const std::filesystem::path& request_path,
    const std::filesystem::path& cwd) noexcept {
  const std::filesystem::path fs_path = cwd / request_path;
  std::unique_ptr<Response> response = ConstructResponseObject(fs_path);

  const std::string& header = ConstructResponseHeader(*response);
  const std::vector<std::byte>& body = response->GetBody();

  std::vector<std::byte> response_bytes;
  response_bytes.reserve(header.size() + body.size());

  std::ranges::transform(
      header.cbegin(), header.cend(), std::back_inserter(response_bytes),
      [](const char character) { return static_cast<std::byte>(character); });

  // Separate header and body with CRLF
  response_bytes.emplace_back(static_cast<std::byte>('\r'));
  response_bytes.emplace_back(static_cast<std::byte>('\n'));

  response_bytes.insert(response_bytes.end(), body.cbegin(), body.cend());

  return response_bytes;
}

}  // namespace http::internals