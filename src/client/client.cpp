#include "../common/error_handling.h"
#include "../common/communication.h"

#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>  // exit(EXIT_CODE)
#include <climits> // ULONG_MAX
#include <cstdint> // UINT16_MAX
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctime>
#include <csignal>

#define NUM_PROGRAM_ARGS 3
#define USAGE "Usage: ./client <pseudo> <ip_serveur> <port>\n"
#define MAX_RECEIVE_BUFFER_SIZE 1024

struct Argument {
  char *pseudo;
  char *ServerIp;
  port_t ServerPort;
};

int setup_client_socket_fd(port_t kServerPort, const char *ServerIp);
port_t get_port_from_str(char *port_str);
Argument parse_args(int argc, char **argv);
void handle_all_requests(int client_socket_fd, const char *pseudo);
[[noreturn]] void* listen_to_server(void * arg);
void disconnect(const char * pseudo);

void setup_fd_set(port_t kServerPort,
				  int &client_socket_fd,
				  const char *ServerIp,
				  fd_set sockets_fds);

int main(int argc, char **argv) {
  INIT_ERROR_HANDLER_USAGE_MESSAGE(USAGE);
  Argument arg = parse_args(argc, argv);

  const port_t kServerPort = arg.ServerPort;
  const char *kServerIp = arg.ServerIp;
  const char *kClientPseudo = arg.pseudo;

  int client_socket_fd;
  fd_set sockets_fds;

  setup_fd_set(kServerPort, client_socket_fd, kServerIp, sockets_fds);
  handle_all_requests(client_socket_fd, kClientPseudo);
}

void setup_fd_set(const port_t kServerPort,
				  int &client_socket_fd,
				  const char *ServerIp,
				  fd_set sockets_fds) {
  client_socket_fd = setup_client_socket_fd(kServerPort, ServerIp);
  FD_ZERO(&sockets_fds);
  FD_SET(client_socket_fd, &sockets_fds);
}

int setup_client_socket_fd(port_t kServerPort, const char *ServerIp) {
  // create the socket
  int client_socket_fd = TRY(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), EXIT_SOCK_ERROR);

  struct hostent *host = gethostbyname(ServerIp);
  struct sockaddr_in server_address;

  bzero(reinterpret_cast<char *>(&server_address), sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons (kServerPort);
  server_address.sin_addr.s_addr = inet_addr(inet_ntoa(*reinterpret_cast<struct in_addr *>(*host->h_addr_list)));

  TRY(connect(client_socket_fd, reinterpret_cast<struct sockaddr *>(&server_address),
			  sizeof(server_address)), EXIT_OTHER_ERROR);

  return client_socket_fd;
}

port_t get_port_from_str(char *port_str) {
  const unsigned long l_port = strtoul(port_str, nullptr, 10);

  if (l_port == 0 || l_port == ULONG_MAX || l_port > MAX_PORT_NUMBER) {
	ERR_N_EXIT("Specified port is not valid", EXIT_BAD_USAGE_ERROR);
  }

  return static_cast<port_t>(l_port); // the conversion is safe, we checked for bounds
}

Argument parse_args(int argc, char **argv) {

  if (argc != NUM_PROGRAM_ARGS + 1) {
	ERR_N_EXIT("Invalid number of arguments", EXIT_BAD_USAGE_ERROR);
  }

  return {argv[1], argv[2], get_port_from_str(argv[3])};
}

void handle_all_requests(int client_socket_fd, const char *pseudo) {
  pthread_t new_thread;
  pthread_create(&new_thread, nullptr, listen_to_server, &client_socket_fd);
  send(client_socket_fd, pseudo, strlen(pseudo), 0); //send the pseudo

  for (;;) {
	Message message {};
	auto user_input_ptr = fgets(message.message, sizeof(message.message), stdin);
	message.mess_size = strnlen(message.message, sizeof(message.message));
	message.timestamp = time(nullptr);

	if (user_input_ptr == nullptr)
	  break;

	send(client_socket_fd, reinterpret_cast<char *>(&message), sizeof(message), 0);
  }
  disconnect(pseudo);
  pthread_kill(new_thread, SIGINT);
}

[[noreturn]] void * listen_to_server(void * arg){
  for(;;){
      int * server_socket_fd = static_cast<int *>(arg);

      char buffer[MAX_MESS_SIZE + 1];
      bzero(buffer, MAX_MESS_SIZE);
      read(*server_socket_fd, buffer, MAX_MESS_SIZE);

	  if (strcmp(buffer, SERVER_SIGINT_MESSAGE) != 0)
		printf ("%s", buffer);
	  else{
		printf("Lost connection...\nServer has stopped working X0\n");
		exit(NORMAL_EXIT);
	  }
  }
}

void disconnect(const char * pseudo){
  printf("Client %s is disconnected\n", pseudo);
  exit(NORMAL_EXIT);
}


