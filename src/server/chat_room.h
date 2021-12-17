//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_SERVER_CHAT_ROOM_H_
#define PROJETCHAT_SRC_SERVER_CHAT_ROOM_H_

#include "server_usage.h"
#include "../common/communication.h"
#include "client.h"
#include "clients_set.h"

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <queue>



class ChatRoom {
 public:
  explicit ChatRoom(long port);

  [[noreturn]] void Listen();
  std::vector<Client *> GetFdsReadyForIO();
 private:
  void HandleClientReadForIO(Client &client);
  static int SetupMasterSocket(port_t kServerPort);
  void HandleNewClientConnection();
  static std::string GetMessageReprFrom(const Message &message, const Username &username);
  void SendAllMessages();
  void AddMessageToSendQueue(const std::string &message);
  void SendToAll(const std::string &message);

  ClientsSet clients_set_;
  std::queue<std::string> pending_messages_;
};


#endif //PROJETCHAT_SRC_SERVER_CHAT_ROOM_H_
