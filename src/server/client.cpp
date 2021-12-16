//
// Created by Anton Romanova on 16/12/2021.
//

#include "client.h"
int Client::GetSocketFd() const {
  return socket_fd_;
}
const std::optional<Username> &Client::GetUsername() const {
  return username_;
}
void Client::SetUsername(Username username) {
  username_ = username;
}
bool Client::HasUsername() const {
  return GetUsername().has_value();
}
bool Client::operator==(const Client &rhs) const {
  return socket_fd_ == rhs.socket_fd_ &&
	  username_ == rhs.username_;
}
bool Client::operator!=(const Client &rhs) const {
  return !(rhs == *this);
}
Client::Client(Client &&other) noexcept {
  socket_fd_ = other.socket_fd_;
  username_ = std::move(other.username_);
}
Client::Client(const Client &other) {
  *this = other;
}
Client &Client::operator=(const Client &other) {
  if (this != &other){
	socket_fd_ = other.socket_fd_;
	username_ = other.username_;
  }
  return *this;
}
