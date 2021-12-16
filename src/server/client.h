//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_SERVER_CLIENT_H_
#define PROJETCHAT_SRC_SERVER_CLIENT_H_

#include <string>

class Client {
 public:
  Client(int socket_fd, std::string username) : socket_fd_(socket_fd), username_(std::move(username)) {}
  [[nodiscard]] int GetSocketFd() const {
	return socket_fd_;
  }
  [[nodiscard]] const std::string &GetUsername() const {
	return username_;
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
  std::string username_;
};

#endif //PROJETCHAT_SRC_SERVER_CLIENT_H_
