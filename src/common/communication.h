//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_COMMON_COMMUNICATION_H_
#define PROJETCHAT_SRC_COMMON_COMMUNICATION_H_

#include <cstring>
#include <ctime>
#include <cstdint> // UINT16_MAX
#include <sys/types.h>
#include <string>
#include <optional>
#include <memory>

#define SETSOCKOPT_REUSABLE 1
#define MAX_MESS_SIZE 1024
#define SOCKETS_BACKLOG 64
#define MAX_PORT_NUMBER UINT16_MAX

// 0s 1000us
#define SELECT_TIMEOUT {0, 1000}

#define SERVER_SIGINT_MESSAGE "Server is about to die :0. See ya'"

typedef uint16_t port_t;

struct Message {
  size_t mess_size;
  time_t timestamp;
  char message[MAX_MESS_SIZE - sizeof(mess_size) - sizeof(timestamp)];

  [[nodiscard]] std::string GetFormattedTime() const{
	const char * time_c_str = ctime(&timestamp);

	if (time_c_str == nullptr) {
	  time_c_str = "Unknown time\n";
	}

	return time_c_str;
  }
};

#endif //PROJETCHAT_SRC_COMMON_COMMUNICATION_H_
