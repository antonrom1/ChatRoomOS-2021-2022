//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_COMMON_ERROR_HANDLING_H_
#define PROJETCHAT_COMMON_ERROR_HANDLING_H_

#include "exit_codes.h"

#define HANDLE_CALL_ERRORS(call, exit_code) error_handler(call, #call, exit_code)
#define guard(condition) if (not (condition)) {return;}

void log_err_and_exit(const char *error_message, int exit_code);
int error_handler(int ret, const char *fun_name, int exit_code);

#endif //PROJETCHAT_COMMON_ERROR_HANDLING_H_
