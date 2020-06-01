#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int compare_ints(const void *a, const void *b)
{
  return (*((int *)b) - *((int *)a));
}

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in servaddr;

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
  {
    servaddr.sin_port = 0;
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
      perror(NULL);
      close(sockfd);
      exit(1);
    }
  }
  socklen_t servlen = sizeof(servaddr);
  listen(sockfd, 5);

  getsockname(sockfd, (struct sockaddr *)&servaddr, &servlen);
  printf("port: %d\n", ntohs(servaddr.sin_port));

  while (1)
  {
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
    int len;
    read(newsockfd, &len, sizeof(int));

    int line[len];
    read(newsockfd, line, len * sizeof(int));
    qsort(line, len, sizeof(int), compare_ints);
    write(newsockfd, line, len * sizeof(int));
  }
}