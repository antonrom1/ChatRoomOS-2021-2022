//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_SERVER_CLIENT_H_
#define PROJETCHAT_SRC_SERVER_CLIENT_H_

#include "username.h"

#include <string>
#include <optional>

class Client {
 public:

  explicit Client(int socket_fd) : socket_fd_(socket_fd) {}

  [[nodiscard]] int GetSocketFd() const;

  [[nodiscard]] const std::optional<Username> &GetUsername() const;

  void SetUsername(Username username);

  [[nodiscard]] bool HasUsername() const;

  bool operator==(const Client &rhs) const;

  bool operator!=(const Client &rhs) const;

  Client(const Client &);
  Client(Client &&) noexcept;
  Client& operator=(const Client &other);

 private:
  int socket_fd_;
  std::optional<Username> username_ {};
};

#endif //PROJETCHAT_SRC_SERVER_CLIENT_H_
