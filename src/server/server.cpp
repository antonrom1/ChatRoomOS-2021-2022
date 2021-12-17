
#include "server_usage.h"
#include "../common/error_handling.h"
#include "chat_room.h"

#include <unistd.h>
#include <cstdlib>  // exit(EXIT_CODE)
#include <climits> // ULONG_MAX

#define NUM_PROGRAM_ARGS 1

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


int main(int argc, char **argv) {
  const port_t kServerPort = parse_args(argc, argv);
  auto cr = ChatRoom::Start(kServerPort);
  cr->Listen();
}