// Copyright 2025 powersagitar
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#include <http/core/http.h>
#include <spdlog/spdlog.h>

#include <boost/program_options.hpp>
#include <cstdint>
#include <filesystem>

[[noreturn]] int main(const int argc, const char *const *const argv) {
  boost::program_options::options_description options;

  // clang-format off
  options.add_options()
    ("cwd",
      boost::program_options::value<std::filesystem::path>()->default_value(std::filesystem::current_path()),
      "Command working directory. Where request paths are relative to."
    )(
      "port",
      boost::program_options::value<uint16_t>()->default_value(80),
      "Port the server listens on."
    )(
      "buffer-size",
      boost::program_options::value<size_t>()->default_value(1024),
      "Size of the buffer used to read from TCP connections."
    )(
      "log-level",
      boost::program_options::value<int>()->default_value(spdlog::level::info),
      "Log level."
    );
  // clang-format on

  boost::program_options::variables_map variables;

  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, options),
      variables);

  boost::program_options::notify(variables);

  spdlog::set_level(
      static_cast<spdlog::level::level_enum>(variables["log-level"].as<int>()));

  const http::Config config{
      .buffer_size = variables["buffer-size"].as<size_t>(),
      .port = variables["port"].as<uint16_t>(),
      .cwd = variables["cwd"].as<std::filesystem::path>()};

  http::Start(config);
}