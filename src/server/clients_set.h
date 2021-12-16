//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_SERVER_CLIENTS_SET_H_
#define PROJETCHAT_SRC_SERVER_CLIENTS_SET_H_

#include "client.h"
#include <sys/select.h>
#include <vector>

class ClientsSet {
 public:
  explicit ClientsSet(int master_fd) : master_fd_(master_fd) {
	Clear();
	FD_SET(master_fd, &clients_fds_);
  }

  void AddClient(const Client &client);
  void RemoveClient(const Client &client);
  void Clear();

  [[nodiscard]] const fd_set &GetClientsFds() const;

  [[nodiscard]] int GetMasterFd() const;

  auto begin() {
	return clients_.begin();
  }

  auto end() {
	return clients_.end();
  }

 private:
  int master_fd_;
  fd_set clients_fds_ {};
  std::vector<Client> clients_;
};

#endif //PROJETCHAT_SRC_SERVER_CLIENTS_SET_H_
