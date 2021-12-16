//
// Created by Anton Romanova on 16/12/2021.
//

#include "chat_room.h"
#include "../common/error_handling.h"



ChatRoom::ChatRoom(long port) : clients_set_(ChatRoom::SetupMasterSocket(port)) {}


[[noreturn]] void ChatRoom::Listen() {
  for (;;) {
	auto clients = GetFdsReadyForIO();
	for (auto &client : clients) {
	  HandleClientReadForIO(*client);
	}
  }
}


std::vector<Client *> ChatRoom::GetFdsReadyForIO() {
  fd_set fds_ready_for_io = clients_set_.GetClientsFds();

  HANDLE_CALL_ERRORS(select(FD_SETSIZE, &fds_ready_for_io, nullptr, nullptr, nullptr), EXIT_SOCK_ERROR);

  if (FD_ISSET(clients_set_.GetMasterFd(), &fds_ready_for_io)) {
	// we have a new connection
	HandleNewClientConnection();
  }

  std::vector<Client*> clients_ready_for_io;
  for (Client &client : clients_set_) {
	if (FD_ISSET(client.GetSocketFd(), &fds_ready_for_io)) {
	  clients_ready_for_io.push_back(&client);
	}
  }
  return clients_ready_for_io;
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
  int new_client_fd = accept(clients_set_.GetMasterFd(), static_cast<sockaddr *>(nullptr), nullptr);
  clients_set_.AddClient(Client{new_client_fd});
}


void ChatRoom::HandleClientReadForIO(Client &client) {
  char receive_buffer[MAX_MESS_SIZE + 1];
  char output_buffer[MAX_MESS_SIZE + 1];

  bzero(receive_buffer, MAX_MESS_SIZE);
  bzero(output_buffer, MAX_MESS_SIZE);

  ssize_t num_bytes_written_to_buffer;
  while ((num_bytes_written_to_buffer = read(client.GetSocketFd(), receive_buffer, MAX_MESS_SIZE - 1)) > 0) {

	if (!client.HasUsername()) {
	  // sends a message
	  client.SetUsername(std::string(receive_buffer));
	} else {
	  // sends username
	  printf("%s a envoyé: %s\n", client.GetUsername().value().c_str(), receive_buffer);
	}


	// TODO: better termination detection
	if (receive_buffer[num_bytes_written_to_buffer - 1] == '\n') {
	  break;
	}

	bzero(receive_buffer, MAX_MESS_SIZE);
  }
  if (num_bytes_written_to_buffer <= 0) {
	clients_set_.RemoveClient(client);
  }

  snprintf(output_buffer,
		   sizeof(output_buffer),
		   "Hello world!");
  write(client.GetSocketFd(), output_buffer, strlen(output_buffer));
}
