//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_COMMON_ERROR_HANDLING_H_
#define PROJETCHAT_COMMON_ERROR_HANDLING_H_

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#ifndef USAGE
#error "USAGE must be defined"
#endif

#define PRINT_USAGE fprintf(stderr, USAGE)
#define HANDLE_CALL_ERRORS(call, exit_code) error_handler(call, #call, exit_code)

#define EXIT_BAD_USAGE_ERROR 1
#define EXIT_SOCK_ERROR 2
#define EXIT_OTHER_ERROR 3

[[noreturn]] void log_err_and_exit(const char *error_message, int exit_code);
int error_handler(int ret, const char *fun_name, int exit_code);
void print_usage() {
  PRINT_USAGE;
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

#endif //PROJETCHAT_COMMON_ERROR_HANDLING_H_
