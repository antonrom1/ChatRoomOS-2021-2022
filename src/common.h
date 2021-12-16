//
// Created by Anton Romanova on 15/12/2021.
//

#ifndef PROJETCHAT_SRC_COMMON_H_
#define PROJETCHAT_SRC_COMMON_H_

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#define EXIT_BAD_USAGE_ERROR 1
#define EXIT_SOCK_ERROR 2
#define EXIT_OTHER_ERROR 3

#define PRINT_USAGE fprintf(stderr, USAGE)
#define HANDLE_CALL_ERRORS(call, exit_code) error_handler(call, #call, exit_code)

struct MessageData {
  time_t timestamp;
  ssize_t mess_size;
  char* message;
};

void message_to_str(MessageData *msg, char* buff, unsigned max_message_size) {
  // TODO: that's ugly and hacky code, we'd better fix it
  auto timestamp_and_mess_size_addr_in_buff = buff;
  auto msg_addr_in_buff = timestamp_and_mess_size_addr_in_buff + sizeof(msg->mess_size);

  memcpy(timestamp_and_mess_size_addr_in_buff, &msg->timestamp, sizeof(msg->timestamp) + sizeof(msg->mess_size));
  memcpy(msg_addr_in_buff, msg->message, strnlen(msg->message, max_message_size));
}

[[noreturn]] void log_err_and_exit(const char *error_message, int exit_code) {
  fprintf(stderr, "%s\n", error_message);

  if (exit_code == EXIT_BAD_USAGE_ERROR) {
	fprintf(stderr, "\n");
	PRINT_USAGE;
  }

  exit(exit_code);
}

int error_handler(int ret, const char *fun_name, int exit_code) {
  if (ret < 0) {
	char *error_string;

	if (asprintf(&error_string, "calling %s returned %i (%d : %s)", fun_name, ret, errno, strerror(errno)) < 0)
	  log_err_and_exit("Error occurred during error handling. asprinf call failed", EXIT_OTHER_ERROR);
	else
	  log_err_and_exit(error_string, exit_code);
  }
  return ret;
}

#endif //PROJETCHAT_SRC_COMMON_H_
