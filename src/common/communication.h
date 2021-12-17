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

#define SETSOCKOPT_REUSABLE 1
#define MAX_MESS_SIZE 1024
#define SOCKETS_BACKLOG 64
#define MAX_PORT_NUMBER UINT16_MAX

typedef uint16_t port_t;

struct Message {
  ssize_t mess_size;
  time_t timestamp;
  char message[MAX_MESS_SIZE - sizeof(mess_size) - sizeof(timestamp)];
};

#endif //PROJETCHAT_SRC_COMMON_COMMUNICATION_H_
