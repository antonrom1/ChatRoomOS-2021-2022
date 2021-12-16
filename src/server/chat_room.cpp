//
// Created by Anton Romanova on 16/12/2021.
//

#include "chat_room.h"

ChatRoom::ChatRoom(long port) : master_fd_(ChatRoom::SetupMasterSocket(port)), clients_fds_(), clients_() {
  Clear();
}
void ChatRoom::AddClient(const Client &client) {
  FD_SET(client.GetSocketFd(), &clients_fds_);
  clients_.push_back(client);
}
void ChatRoom::RemoveClient(const Client &client) {
  FD_CLR(client.GetSocketFd(), &clients_fds_);
  clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
}
void ChatRoom::Listen() {
  for (auto &client : GetFdsReadyForIO()) {

  }
}
std::vector<Client> ChatRoom::GetFdsReadyForIO() {
  fd_set fds_ready_for_io = GetClientsFds();

  // TODO: Replace FD_SETSIZE with a more optimized version. This way we won't iterate 1024 times uselessly
  HANDLE_CALL_ERRORS(select(FD_SETSIZE, &fds_ready_for_io, nullptr, nullptr, nullptr), EXIT_SOCK_ERROR);

  if (FD_ISSET(GetMasterFd(), &fds_ready_for_io)) {
	// we have a new connection
	HandleNewClientConnection();
  }

  std::vector<Client> clients_ready_for_io;
  for (const Client &client : clients_) {
	if (FD_ISSET(client.GetSocketFd(), &fds_ready_for_io)) {
	  clients_ready_for_io.push_back(client);
	}
  }
  return clients_ready_for_io;
}
int ChatRoom::GetMasterFd() const {
  return master_fd_;
}
int ChatRoom::SetupMasterSocket(port_t kServerPort) {
  // create the socket
  int master_socket_fd = HANDLE_CALL_ERRORS(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), EXIT_SOCK_ERROR);

  // make the port reusable
  int opt = SETSOCKOPT_REUSABLE;
  HANDLE_CALL_ERRORS(setsockopt(master_socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)),
					 EXIT_SOCK_ERROR);

  // setup socket address
  struct sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY); // conform the endianness with IEEE Std POSIX.1-200x
  server_address.sin_port = htons(kServerPort);

  // bind and listen
  HANDLE_CALL_ERRORS(bind(master_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)),
					 EXIT_SOCK_ERROR);
  HANDLE_CALL_ERRORS(listen(master_socket_fd, SOCKETS_BACKLOG), EXIT_SOCK_ERROR);

  return master_socket_fd;
}
void ChatRoom::HandleNewClientConnection() {
  int new_client_fd = accept(master_fd_, static_cast<sockaddr *>(nullptr), nullptr);
  AddClient({new_client_fd, "test"});
}
void ChatRoom::Clear() {
  FD_ZERO(&clients_fds_);
  clients_.clear();
}
const fd_set &ChatRoom::GetClientsFds() const {
  return clients_fds_;
}
