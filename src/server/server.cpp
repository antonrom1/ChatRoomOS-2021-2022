#include "server_usage.h"
#include "../common/error_handling.h"
#include "../common/communication.h"
#include "chat_room.h"

#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>  // exit(EXIT_CODE)
#include <climits> // ULONG_MAX
#include <utility>

#define NUM_PROGRAM_ARGS 1

 // the maximum length for the queue of pending connections.

int setup_master_socket_fd(port_t kServerPort);
void process_client_socket_request(int client_socket_fd, fd_set *all_sockets_fds);
void handle_client_message(int fd, fd_set *selected_sockets_fds, fd_set *all_sockets_fds);
port_t get_port_from_str(char *port_str);
port_t parse_args(int argc, char **argv);
bool close_socket_if_needed(int client_fd, fd_set *fds);
int handle_new_client_connection(int master_socket_fd, fd_set &all_sockets_fds);
[[noreturn]] void handle_all_requests(int master_socket_fd, fd_set &all_sockets_fds);
void setup_fd_set(const port_t &kServerPort, int &master_socket_fd, fd_set &sockets_fds);
void close_socket(int client_fd, fd_set *fds);


int main(int argc, char **argv) {
  const port_t kServerPort = parse_args(argc, argv);
//  int master_socket_fd;
//  fd_set sockets_fds;
//
//  setup_fd_set(kServerPort, master_socket_fd, sockets_fds);
//  handle_all_requests(master_socket_fd, sockets_fds);

  auto cr = ChatRoom(kServerPort);

  cr.Listen();

//  for (auto &client : cr.GetFdsReadyForIO() ) {
//	handle_client_message(client.GetSocketFd(), )
//  }

}

void setup_fd_set(const port_t &kServerPort, int &master_socket_fd, fd_set &sockets_fds) {
  master_socket_fd = setup_master_socket_fd(kServerPort);
  FD_ZERO(&sockets_fds);
  FD_SET(master_socket_fd, &sockets_fds);
}

[[noreturn]] void handle_all_requests(int master_socket_fd, fd_set &all_sockets_fds) {
  fd_set selected_sockets_fds;
  for (;;) {
	selected_sockets_fds = all_sockets_fds;

	// TODO: Replace FD_SETSIZE with a more optimized version. This way we won't iterate 1024 times uselessly
	HANDLE_CALL_ERRORS(select(FD_SETSIZE, &selected_sockets_fds, NULL, NULL, NULL), EXIT_SOCK_ERROR);

	if (FD_ISSET(master_socket_fd, &selected_sockets_fds)) {
	  // we have a new connection
	  handle_new_client_connection(master_socket_fd, all_sockets_fds);
	}

	for (int i = 0; i < FD_SETSIZE; i++) {
	  if (i != master_socket_fd && FD_ISSET(i, &selected_sockets_fds)) {
		// we have a new message
		handle_client_message(i, &selected_sockets_fds, &all_sockets_fds);
	  }
	}
  }
}

int handle_new_client_connection(int master_socket_fd, fd_set &all_sockets_fds) {
  int new_client_fd = accept(master_socket_fd, static_cast<sockaddr *>(nullptr), nullptr);
  FD_SET(new_client_fd, &all_sockets_fds);
  return new_client_fd;
}

void handle_client_message(int fd, fd_set *selected_sockets_fds, fd_set *all_sockets_fds) {
  if (FD_ISSET(fd, selected_sockets_fds)) {
	process_client_socket_request(fd, all_sockets_fds);
	close_socket_if_needed(fd, all_sockets_fds);
  }
}

int setup_master_socket_fd(port_t kServerPort) {
  // create the socket
  int master_socket_fd = HANDLE_CALL_ERRORS(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), EXIT_SOCK_ERROR);

  // make the port reusable
  int opt = 1;
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

port_t get_port_from_str(char *port_str) {
  const unsigned long l_port = strtoul(port_str, nullptr, 10);

  if (l_port == 0 || l_port == ULONG_MAX || l_port > MAX_PORT_NUMBER) {
	log_err_and_exit("Specified port is not valid", EXIT_BAD_USAGE_ERROR);
  }

  return static_cast<port_t>(l_port); // the conversion is safe, we checked for bounds
}

port_t parse_args(int argc, char **argv) {

  if (argc != NUM_PROGRAM_ARGS + 1) {
	log_err_and_exit("Invalid number of arguments", EXIT_BAD_USAGE_ERROR);
  }

  return get_port_from_str(argv[1]);
}

void process_client_socket_request(int client_socket_fd, fd_set *all_sockets_fds) {

}


void close_socket(int client_fd, fd_set *fds) {
  close(client_fd);
  FD_CLR(client_fd, fds);
}

bool close_socket_if_needed(int client_fd, fd_set *fds) {
  // TODO: Add a better handler lmao
  return false;
}
