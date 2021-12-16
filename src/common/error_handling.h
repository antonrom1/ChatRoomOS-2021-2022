//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_COMMON_ERROR_HANDLING_H_
#define PROJETCHAT_COMMON_ERROR_HANDLING_H_

#define PRINT_USAGE fprintf(stderr, USAGE)
#define HANDLE_CALL_ERRORS(call, exit_code) error_handler(call, #call, exit_code)

#define EXIT_BAD_USAGE_ERROR 1
#define EXIT_SOCK_ERROR 2
#define EXIT_OTHER_ERROR 3

[[noreturn]] void log_err_and_exit(const char *error_message, int exit_code);
int error_handler(int ret, const char *fun_name, int exit_code);

#endif //PROJETCHAT_COMMON_ERROR_HANDLING_H_
