//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_SERVER_CLIENT_H_
#define PROJETCHAT_SRC_SERVER_CLIENT_H_

#include <string>
#include <optional>

class Client {
 public:

  explicit Client(int socket_fd) : socket_fd_(socket_fd) {}

  [[nodiscard]] int GetSocketFd() const {
	return socket_fd_;
  }

  [[nodiscard]] const std::optional<std::string> &GetUsername() const {
	return username_;
  }

  void SetUsername(const std::string &username) {
	username_ = username;
  }

  [[nodiscard]] bool HasUsername() const {
	return GetUsername().has_value();
  }

  bool operator==(const Client &rhs) const {
	return socket_fd_ == rhs.socket_fd_ &&
		username_ == rhs.username_;
  }

  bool operator!=(const Client &rhs) const {
	return !(rhs == *this);
  }

 private:
  int socket_fd_;
  std::optional<std::string> username_;
};

#endif //PROJETCHAT_SRC_SERVER_CLIENT_H_
