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
#include <csignal>



class ChatRoom {
 public:
  static ChatRoom *Start(long port);
  static ChatRoom *GetShared();

  void Listen();
  std::vector<Client *> GetFdsReadyForIO();
 private:
  explicit ChatRoom(long port);

  static unsigned SetupMasterSocket(port_t kServerPort);
  static void SigHandler(int sig);

  static ChatRoom *shared_;

  static void SetShared(ChatRoom *shared);

  void HandleClientReadForIO(Client &client);
  void HandleNewClientConnection();
  static std::string GetMessageReprFrom(const Message &message, const Username &username);
  void SendAllMessages();
  void AddMessageToSendQueue(const std::string &message);
  void SendToAll(const std::string &message);
  void Stop();

  ClientsSet clients_set_;
  std::queue<std::string> pending_messages_ {};
  volatile sig_atomic_t should_stop_ = 0;
};


#endif //PROJETCHAT_SRC_SERVER_CHAT_ROOM_H_
