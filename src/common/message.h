//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_COMMON_MESSAGE_H_
#define PROJETCHAT_SRC_COMMON_MESSAGE_H_

#include <cstring>
#include <time.h>

#define MAX_MESS_SIZE 1024

struct Message {
  ssize_t mess_size;
  time_t timestamp;
  char message[MAX_MESS_SIZE];
};

#endif //PROJETCHAT_SRC_COMMON_MESSAGE_H_
