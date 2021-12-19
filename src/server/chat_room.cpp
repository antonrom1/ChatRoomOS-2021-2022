//
// Created by Anton Romanova on 16/12/2021.
//

#include <csignal>
#include "chat_room.h"
#include "../common/error_handling.h"
#include "username.h"

ChatRoom::ChatRoom(long port) : clients_set_(ChatRoom::SetupMasterSocket(static_cast<port_t>(port))) {
  signal(SIGINT, ChatRoom::SigHandler);
  signal(SIGPIPE, SIG_IGN);
}

void ChatRoom::Listen() {

  while (!should_stop_) {
	auto clients = GetFdsReadyForIO();
	for (auto &client : clients) {
	  HandleClientReadForIO(*client);
	}
	SendAllMessages();
  }
  SendToAll(SERVER_SIGINT_MESSAGE);

}

std::vector<Client *> ChatRoom::GetFdsReadyForIO() {
  fd_set fds_ready_for_io = clients_set_.GetClientsFds();
  struct timeval select_timeout = SELECT_TIMEOUT;
  auto select_ret = select(FD_SETSIZE, &fds_ready_for_io, nullptr, nullptr, &select_timeout);

  if (select_ret < 0) {
	if (errno != EINTR) {
	  ERR_N_EXIT("Error running select", EXIT_SOCK_ERROR);
	} else {
	  return {};
	}
  }

  if (FD_ISSET(clients_set_.GetMasterFd(), &fds_ready_for_io)) {
	// we have a new connection
	HandleNewClientConnection();
  }

  std::vector<Client *> clients_ready_for_io;
  for (Client &client : clients_set_) {
	if (FD_ISSET(client.GetSocketFd(), &fds_ready_for_io)) {
	  clients_ready_for_io.push_back(&client);
	}
  }
  return clients_ready_for_io;
}

unsigned ChatRoom::SetupMasterSocket(port_t kServerPort) {
  // create the socket
  int master_socket_fd = TRY(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), EXIT_SOCK_ERROR);

  // make the port reusable
  int opt = SETSOCKOPT_REUSABLE;
  TRY(setsockopt(master_socket_fd,
				 SOL_SOCKET,
				 SO_REUSEADDR,
				 reinterpret_cast<char *>(&opt),
				 sizeof(opt)),
	  EXIT_SOCK_ERROR);

  // setup socket address
  struct sockaddr_in server_address{};
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY); // conform the endianness with IEEE Std POSIX.1-200x
  server_address.sin_port = htons(kServerPort);

  // bind and listen
  TRY(bind(master_socket_fd,
		   reinterpret_cast<struct sockaddr *>(&server_address),
		   sizeof(server_address)),
	  EXIT_SOCK_ERROR);
  TRY(listen(master_socket_fd, SOCKETS_BACKLOG), EXIT_SOCK_ERROR);

  return static_cast<unsigned>(master_socket_fd);
}

void ChatRoom::HandleNewClientConnection() {
  int new_client_fd = accept(static_cast<int>(clients_set_.GetMasterFd()), static_cast<sockaddr *>(nullptr), nullptr);
  clients_set_.AddClient(Client{new_client_fd});
}

void ChatRoom::HandleClientReadForIO(Client &client) {
  char receive_buffer[MAX_MESS_SIZE + 1];
  char output_buffer[MAX_MESS_SIZE + 1];

  bzero(receive_buffer, MAX_MESS_SIZE);
  bzero(output_buffer, MAX_MESS_SIZE);

  ssize_t num_bytes_written_to_buffer = read(client.GetSocketFd(), receive_buffer, MAX_MESS_SIZE);
  if (num_bytes_written_to_buffer > 0) {
	if (!client.HasUsername()) {
	  // The user has to send his (or her) username
	  client.SetUsername(Username(receive_buffer));
	} else {
	  // the user has already given his/her username, the buffer should represent an actual message
	  Message message = *(reinterpret_cast<Message *>(receive_buffer));
	  AddMessageToSendQueue(GetMessageReprFrom(message, client.GetUsername().value()));
	}
  } else if (num_bytes_written_to_buffer <= 0) {
	// The client might have some problems...
	clients_set_.RemoveClient(client);
  }
}

void ChatRoom::AddMessageToSendQueue(const std::string &message) {
  pending_messages_.push(message);
}

std::string ChatRoom::GetMessageReprFrom(const Message &message, const Username &username) {
  auto time = message.GetFormattedTime();

  return username.GetValue() + " sent a message at " + time + "\t" + "> " + message.message;
}

void ChatRoom::SendAllMessages() {
  while (!pending_messages_.empty()) {
	SendToAll(pending_messages_.front());
	pending_messages_.pop();
  }
}

void ChatRoom::SendToAll(const std::string &message) {
  for (auto &client : clients_set_) {
	send(client.GetSocketFd(), message.c_str(), std::min(message.size(), static_cast<unsigned long>(MAX_MESS_SIZE)), 0);
  }
}

void ChatRoom::SigHandler(int sig) {
  guard(sig != SIGPIPE)
  if (sig == SIGINT) {
	if (GetShared() != nullptr) {
	  GetShared()->Stop();
	}
  }
}

ChatRoom *ChatRoom::Create(long port) {
  SetShared(new ChatRoom(port));
  return GetShared();
}

ChatRoom *ChatRoom::GetShared() {
  return ChatRoom::shared_;
}

void ChatRoom::SetShared(ChatRoom *shared) {
  shared_ = shared;
}

void ChatRoom::Stop() {
  should_stop_ = true;
}

ChatRoom *ChatRoom::shared_ = nullptr;
