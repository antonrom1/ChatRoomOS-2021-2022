//
// Created by Anton Romanova on 16/12/2021.
//

#include "username.h"

Username::Username(const std::string &value) : value_(Clean(value)) {}

std::string Username::Clean(const std::string &username) {
  std::string clean_output = {};
  for (auto &c : username) {
	if (std::isalnum(c)) {
	  clean_output += c;
	}
  }

  return clean_output;
}

const std::string &Username::GetValue() const {
  return value_;
}

Username::operator std::string() const {
  return GetValue();
}

bool Username::operator==(const Username &rhs) const {
  return value_ == rhs.value_;
}

bool Username::operator!=(const Username &rhs) const {
  return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Username &username) {
  os << "value_: " << username.value_;
  return os;
}

Username::Username(const Username &other) {
  *this = other;
}
Username::Username(Username && username) noexcept{
  value_ = std::move(username.value_);
}
Username &Username::operator=(const Username &other) {
  if (this != &other) {
	value_ = other.value_;
  }
  return *this;
}
