//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_SERVER_USERNAME_H_
#define PROJETCHAT_SRC_SERVER_USERNAME_H_

#include <string>
#include <utility>
#include <ostream>

class Username {
 public:
  explicit Username(const std::string& value);
  Username(const Username &);
  Username(Username &&) noexcept;

  explicit operator std::string() const;

  [[nodiscard]] const std::string &GetValue() const;

  Username& operator=(const Username& other);
  bool operator==(const Username &rhs) const;
  bool operator!=(const Username &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const Username &username);

 private:

  static std::string Clean(const std::string& username);

 private:
  std::string value_;
};

#endif //PROJETCHAT_SRC_SERVER_USERNAME_H_
