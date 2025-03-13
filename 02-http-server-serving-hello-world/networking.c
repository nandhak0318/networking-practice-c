#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

char ipstr[INET6_ADDRSTRLEN];

int getaddrinfo(const char *node,    // e.g. "www.example.com" or IP
                const char *service, // e.g. "http" or port number
                const struct addrinfo *hints, struct addrinfo **res);

int main() {

  // variables for getaddrinfo
  struct addrinfo hints;
  struct addrinfo *serverInfo;
  int error;

  // initialize hints with 0
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  error = getaddrinfo(NULL, "3030", &hints, &serverInfo);
  if (error) {
    exit(1);
  }

  // creating a socket
  int fd = socket(PF_INET, SOCK_STREAM, 0);
  printf("%d", fd);

  // creating bind
  int bindCode = bind(fd, (struct sockaddr *)serverInfo->ai_addr,
                      serverInfo->ai_addr->sa_len);
  printf("%d", bindCode);

  // listening to a new fd for request
  listen(fd, 10);

  // accept new request
  struct sockaddr_storage their_addr;
  socklen_t addr_size = sizeof(their_addr);

  int new_fd = accept(fd, serverInfo->ai_addr, &addr_size);

  char sendbuff[] =
      "HTTP/1.1 200 OK\nContent-Type: text/html; "
      "charset=UTF-8\nContent-Length: 1256\n\nhello world\r\n\r\n ";
  int sendStatus = send(new_fd, sendbuff, sizeof(sendbuff), 0);

  // close fds and free memory
  close(fd);
  close(new_fd);
  freeaddrinfo(serverInfo);
}
