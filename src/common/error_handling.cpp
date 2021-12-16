//
// Created by Anton Romanova on 16/12/2021.
//

#include "error_handling.h"
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

void log_err_and_exit(const char *error_message, int exit_code) {
  fprintf(stderr, "%s\n", error_message);
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
