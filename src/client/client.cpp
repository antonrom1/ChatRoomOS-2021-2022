#define USAGE "Usage: ./client <pseudo> <ip_serveur> <port>\n"

#include "../common/error_handling.h"
#include "../common/message.h"

#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>  // exit(EXIT_CODE)
#include <climits> // ULONG_MAX
#include <cstdint> // UINT16_MAX
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_PORT_NUMBER UINT16_MAX
#define NUM_PROGRAM_ARGS 3

#define SOCKETS_BACKLOG 64 // the maximum length for the queue of pending connections.
#define MAX_RECEIVE_BUFFER_SIZE 1024

typedef uint16_t port_t;

struct client_parameters_t{
  char * pseudo;
  char * ServerIp;
  port_t ServerPort;
};

int setup_client_socket_fd(int &client_address);
port_t get_port_from_str(char *port_str);
client_parameters_t parse_args(int argc, char **argv);

void setup_fd_set(port_t kServerPort,
                  int &client_socket_fd,
                  const char * ServerIp,
                  const char * ClientPseudo);

int main(int argc, char **argv) {
  client_parameters_t arg = parse_args(argc, argv);

  const port_t kServerPort = arg.ServerPort;
  const char *kServerIp = arg.ServerIp;
  const char *kClientPseudo = arg.pseudo;


  int client_socket_fd;


  setup_fd_set(kServerPort, client_socket_fd, kServerIp, kClientPseudo);

  return 0;
}

void setup_fd_set(const port_t kServerPort,
                  int &client_socket_fd,
                  const char * ServerIp,
                  const char * ClientPseudo) {
  struct hostent* host = gethostbyname (ServerIp);

  struct sockaddr_in client_address;
  bzero((char*)&client_address, sizeof(client_address));

  client_address.sin_family = AF_INET;
  client_address.sin_addr.s_addr = inet_addr(inet_ntoa (*(struct in_addr*)*host->h_addr_list));
  client_address.sin_port = htons (kServerPort);

  client_socket_fd= setup_client_socket_fd(client_socket_fd);

}

int setup_client_socket_fd(int &client_socket_fd) {
  // create the socket
  client_socket_fd = HANDLE_CALL_ERRORS(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), EXIT_SOCK_ERROR);


  HANDLE_CALL_ERRORS(connect (client_socket_fd, (sockaddr*) &client_address,
                                           sizeof(client_address)), EXIT_SOCK_ERROR);

  return client_socket_fd;
}

port_t get_port_from_str(char *port_str) {
  const unsigned long l_port = strtoul(port_str, nullptr, 10);

  if (l_port == 0 || l_port == ULONG_MAX || l_port > MAX_PORT_NUMBER) {
      log_err_and_exit("Specified port is not valid", EXIT_BAD_USAGE_ERROR);
    }

  return static_cast<port_t>(l_port); // the conversion is safe, we checked for bounds
}

client_parameters_t parse_args(int argc, char **argv) {

  if (argc != NUM_PROGRAM_ARGS + 1) {
      log_err_and_exit("Invalid number of arguments", EXIT_BAD_USAGE_ERROR);
    }

  return {argv[1], argv[2], get_port_from_str(argv[3])};
}

void process_client_socket_request(int client_socket_fd) {
  char receive_buffer[MAX_RECEIVE_BUFFER_SIZE + 1];
  char output_buffer[MAX_RECEIVE_BUFFER_SIZE + 1];

  bzero(receive_buffer, MAX_RECEIVE_BUFFER_SIZE);
  bzero(output_buffer, MAX_RECEIVE_BUFFER_SIZE);

  ssize_t num_bytes_written_to_buffer;
  while (true) {
      // TODO The client have to be able to listen and write simultaneously
      printf("%s\n", receive_buffer);
      if (receive_buffer[num_bytes_written_to_buffer - 1] == '\n') {
          break;
        }

      bzero(receive_buffer, MAX_RECEIVE_BUFFER_SIZE);
    }
  snprintf(output_buffer, sizeof(output_buffer), "HTTP/1.0 200 OK\r\n\r\n<h1 style=\"color:white;font-family:helvetica\">Hello world</h1><style>html {background: black;}</style>");
  write(client_socket_fd, output_buffer, strlen(output_buffer));
}