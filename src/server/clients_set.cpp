//
// Created by Anton Romanova on 16/12/2021.
//

#include "clients_set.h"


const fd_set &ClientsSet::GetClientsFds() const {
  return clients_fds_;
}

void ClientsSet::Clear() {
  FD_ZERO(&clients_fds_);
  clients_.clear();
}

void ClientsSet::AddClient(const Client &client) {
  FD_SET(client.GetSocketFd(), &clients_fds_);
  clients_.push_back(client);
}


void ClientsSet::RemoveClient(const Client &client) {
  FD_CLR(client.GetSocketFd(), &clients_fds_);
  clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
}

int ClientsSet::GetMasterFd() const {
  return master_fd_;
}
