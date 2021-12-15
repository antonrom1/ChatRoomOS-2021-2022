#include <cstdio>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>  // exit(EXIT_CODE)
#include <climits> // ULONG_MAX
#include <cstdint>

#define PRINT_USAGE fprintf(stderr, "Usage: ./serveur <port>\n")

#define BAD_USAGE_EXIT_CODE 1
#define SOCKET_ERROR_EXIT_CODE 2

#define MAX_PORT_NUMBER UINT16_MAX
#define NUM_PROGRAM_ARGS 1

typedef uint16_t port_t;

[[noreturn]] void log_err_and_exit(const char *error_message, int exit_code) {
  fprintf(stderr, "%s\n", error_message);

  if (exit_code == BAD_USAGE_EXIT_CODE) {
	fprintf(stderr, "\n");
	PRINT_USAGE;
  }

  exit(exit_code);
}

port_t get_port_from(char *port_str) {
  const unsigned long l_port = strtoul(port_str, nullptr, 10);
  if (l_port == 0 || l_port == ULONG_MAX || l_port > MAX_PORT_NUMBER) {
	log_err_and_exit("Specified port is not valid", BAD_USAGE_EXIT_CODE);
  }
  return static_cast<port_t>(l_port); // the conversion is safe, we checked for bounds
}

int main(int argc, char **argv) {
  int opt = 1;
  int master_socket;
  int socket_fd;
  struct sockaddr_in server_address{};

  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      log_err_and_exit ("Failed to create socket", SOCKET_ERROR_EXIT_CODE);
  }

  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))){
    log_err_and_exit ("Failed to set socket option", SOCKET_ERROR_EXIT_CODE);
  }


  if (argc != NUM_PROGRAM_ARGS + 1) {
	log_err_and_exit("Invalid number of arguments", BAD_USAGE_EXIT_CODE);
  }

  const port_t SERVER_PORT = get_port_from(argv[1]);

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
	log_err_and_exit("Error creating socket", SOCKET_ERROR_EXIT_CODE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY); // conform the endianness with IEEE Std POSIX.1-200x
  server_address.sin_port = htons(SERVER_PORT);

  if ((bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address))) != 0) {
	log_err_and_exit("Address could not be assigned to the socket", SOCKET_ERROR_EXIT_CODE);
  }

  if ((listen(socket_fd, 64)) != 0) {
	log_err_and_exit("Listen error", SOCKET_ERROR_EXIT_CODE);
  }

  return 0;
}