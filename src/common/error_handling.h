//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_COMMON_ERROR_HANDLING_H_
#define PROJETCHAT_COMMON_ERROR_HANDLING_H_

#include "exit_codes.h"

#include <memory>
#include <optional>
#include <string>
#include <utility>

#define INIT_ERROR_HANDLER_USAGE_MESSAGE(USAGE) ErrorHandler::CreateShared(USAGE)
#define TRY(call, exit_code) ErrorHandler::GetShared()->error_handler(call, #call, exit_code)
#define ERR_N_EXIT(error_message, exit_code) ErrorHandler::GetShared()->log_err_and_exit(error_message, exit_code)
#define guard(condition) if (not (condition)) {return;}

class ErrorHandler {
 public:
  static std::shared_ptr<ErrorHandler> GetShared();
  static const std::shared_ptr<ErrorHandler> &CreateShared(const std::string &usage);

  void log_err_and_exit(const char *error_message, int exit_code);
  int error_handler(int ret, const char *fun_name, int exit_code);

 private:
  static std::shared_ptr<ErrorHandler> shared_;
  explicit ErrorHandler(std::string usage) : usage_message_(std::move(usage)) {};

  std::string usage_message_;
};

#endif //PROJETCHAT_COMMON_ERROR_HANDLING_H_
