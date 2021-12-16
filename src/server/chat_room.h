//
// Created by Anton Romanova on 16/12/2021.
//

#ifndef PROJETCHAT_SRC_SERVER_CHAT_ROOM_H_
#define PROJETCHAT_SRC_SERVER_CHAT_ROOM_H_

#include "client.h"
#include "server_usage.h"
#include "../common/error_handling.h"
#include "../common/communication.h"

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>

class ChatRoom {
 public:
  explicit ChatRoom(long port);
  void AddClient(const Client &client);
  void RemoveClient(const Client &client);
  void Listen();
  std::vector<Client> GetFdsReadyForIO();
  [[nodiscard]] int GetMasterFd() const;

 private:
  void HandleClientReadForIO(const Client &client) {
	char receive_buffer[MAX_MESS_SIZE + 1];
	char output_buffer[MAX_MESS_SIZE + 1];

	bzero(receive_buffer, MAX_MESS_SIZE);
	bzero(output_buffer, MAX_MESS_SIZE);

	ssize_t num_bytes_written_to_buffer;
	while ((num_bytes_written_to_buffer = read(client.GetSocketFd(), receive_buffer, MAX_MESS_SIZE - 1)) > 0) {

	  printf("%s\n", receive_buffer);

	  // TODO: better termination detection
	  if (receive_buffer[num_bytes_written_to_buffer - 1] == '\n') {
		break;
	  }

	  bzero(receive_buffer, MAX_MESS_SIZE);
	}
	if (num_bytes_written_to_buffer <= 0) {
	  RemoveClient(client);
	}


	snprintf(output_buffer,
			 sizeof(output_buffer),
			 "Hello world!");
	write(client.GetSocketFd(), output_buffer, strlen(output_buffer));
  }
  static int SetupMasterSocket(port_t kServerPort);
  void HandleNewClientConnection();
  void Clear();
  [[nodiscard]] const fd_set &GetClientsFds() const;

  int master_fd_;
  fd_set clients_fds_;
  std::vector<Client> clients_;
};

#endif //PROJETCHAT_SRC_SERVER_CHAT_ROOM_H_
