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

void print_addrinfo(struct addrinfo *info) {
  while (info) {
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)info->ai_addr;
    inet_ntop(info->ai_family, &(ipv4->sin_addr), ipstr, sizeof ipstr);
    printf("Family: %d, Socktype: %d, Protocol: %d\n", info->ai_family,
           info->ai_socktype, info->ai_protocol);
    info = info->ai_next; // Move to the next result
    printf("%s \n", ipstr);
  }
}

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

  // connecting to a local network
  int connectInfo = connect(fd, (struct sockaddr *)serverInfo->ai_addr,
                            (socklen_t)serverInfo->ai_addrlen);
  printf("connect Info: %d \n", connectInfo);
  if (connectInfo < 0)
    printf("error: %s \n", gai_strerror(connectInfo));

  char sendbuff[] = "GET / HTTP/1.0\r\n\r\n";
  int sendStatus = send(fd, sendbuff, sizeof(sendbuff), 0);

  char reciveBuff[1000];
  int recivevStatus = recv(fd, reciveBuff, sizeof(reciveBuff), 0);

  printf("recived String %s", reciveBuff);

  close(fd);
  freeaddrinfo(serverInfo);
}
/*int main() {
  int status;
  socklen_t addr_size;
  int new_fd;
  struct sockaddr_storage their_addr;
  struct addrinfo hints;
  struct addrinfo *serverInfo;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  if ((status = getaddrinfo("127.0.0.1", "3490", &hints, &serverInfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
  int fd = socket(serverInfo->ai_family, serverInfo->ai_socktype,
                  serverInfo->ai_protocol);
  print_addrinfo(serverInfo);

  printf("FD: %d\n", fd);

  int bindId = bind(fd, serverInfo->ai_addr, serverInfo->ai_addr->sa_len);
  printf("bindID: %d\n", bindId);
  listen(fd, 10);

  addr_size = sizeof their_addr;
  new_fd = accept(fd, (struct sockaddr *)&their_addr, &addr_size);

  printf("new_fd %d\n", new_fd);
  char *mdg = "maja mamae\n";
  int len = strlen(mdg);
  int bytes_send;
  bytes_send = send(new_fd, mdg, len, 0);

  char buff[100];
  int recvs = recv(new_fd, buff, 100, 0);
  printf("%s \n", buff);

  printf("Byptes send %d\n", bytes_send);

  freeaddrinfo(serverInfo);
  return 0;
} */
