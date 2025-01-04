//
// Created by powersagitar on 1/1/25.
//

#ifndef SOCKET_H
#define SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <system_error>

namespace http {

namespace internals {

constexpr int kNullSock = -1;
constexpr int kHTTPPort = 80;

}  // namespace internals

class SocketClient {
 public:
  SocketClient(const SocketClient &) = delete;
  SocketClient &operator=(const SocketClient &) = delete;

  SocketClient(SocketClient &&obj) noexcept { *this = std::move(obj); }

  SocketClient &operator=(SocketClient &&rhs) noexcept {
    if (this == &rhs) {
      return *this;
    }

    this->sockfd_ = rhs.sockfd_;
    rhs.sockfd_ = internals::kNullSock;

    return *this;
  }

  SocketClient(const int sockfd, const sockaddr_in addr,
               const socklen_t addr_len) noexcept
      : sockfd_(sockfd), addr_(addr), addr_len_(addr_len) {}

  ~SocketClient() noexcept {
    if (sockfd_ == internals::kNullSock) {
      return;
    }

    if (close(sockfd_) != 0) {
      // TODO: error handling / logging
      // throw std::system_error(
      //     errno, std::system_category(),
      //     "Failed to close client socket " + std::to_string(sockfd_));
    }
  }

  ssize_t Read(void *const buf, const ssize_t buf_size) const noexcept {
    return read(sockfd_, buf, buf_size);
  }

 private:
  int sockfd_ = internals::kNullSock;
  sockaddr_in addr_{};
  socklen_t addr_len_{};
};

class SocketServer {
 public:
  /**
   * See man protocols(5)
   */
  enum class Protocol : int {
    kIP = 0,
  };

  /**
   * SocketServer is uniquely owned. No two instances may share the same socket.
   */
  SocketServer(const SocketServer &) = delete;
  SocketServer &operator=(const SocketServer &) = delete;

  SocketServer(SocketServer &&obj) noexcept { *this = std::move(obj); }

  SocketServer &operator=(SocketServer &&rhs) noexcept {
    if (this == &rhs) {
      return *this;
    }

    this->sockfd_ = rhs.sockfd_;
    rhs.sockfd_ = internals::kNullSock;

    return *this;
  }

  /**
   * See man socket(2)
   */
  SocketServer() noexcept
      : sockfd_(socket(PF_INET, SOCK_STREAM, static_cast<int>(Protocol::kIP))) {
  }

  /**
   * See man close(2)
   */
  ~SocketServer() noexcept {
    if (sockfd_ == internals::kNullSock) {
      return;
    }

    if (close(sockfd_) != 0) {
      // TODO: error handling / logging
      // throw std::system_error(
      //     errno, std::system_category(),
      //     "Failed to close server socket " + std::to_string(sockfd_));
    }
  }

  /**
   * See man bind(2)
   */
  [[nodiscard]] int Bind(const sa_family_t sin_family = AF_INET,
                         const in_port_t sin_port = internals::kHTTPPort,
                         const in_addr sin_addr = {INADDR_ANY}) const noexcept {
    sockaddr_in addr{.sin_family = sin_family,
                     .sin_port = sin_port,
                     .sin_addr = sin_addr,
                     .sin_zero{0}};

    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return bind(sockfd_, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
  }

  /**
   * See man listen(2)
   */
  [[nodiscard]] int Listen(const int backlog = 512) const noexcept {
    return listen(sockfd_, backlog);
  }

  /**
   * See man accept(2)
   */
  [[nodiscard]] SocketClient Accept() const {
    sockaddr_in addr{};
    socklen_t addr_len{};

    const int client_sockfd =
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        accept(sockfd_, reinterpret_cast<sockaddr *>(&addr), &addr_len);

    if (client_sockfd == internals::kNullSock) {
      throw std::system_error(errno, std::system_category(),
                              "Failed to accept client socket");
    }

    return {client_sockfd, addr, addr_len};
  }

 private:
  int sockfd_ = internals::kNullSock;
};

}  // namespace http

#endif  // SOCKET_H
