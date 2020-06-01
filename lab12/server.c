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

#define MAX_FILENAME_SIZE 256

long fsize(FILE *fp)
{
  fseek(fp, 0L, SEEK_END);
  long sz = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  return sz;
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
  printf("listening on port: %d\n", ntohs(servaddr.sin_port));

  if (fork() == 0)
  {
    while (1)
    {
      struct sockaddr_in cliaddr;
      socklen_t clilen = sizeof(cliaddr);
      int newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
      if (fork() == 0)
        continue;

      while (1)
      {
        char filename[MAX_FILENAME_SIZE];
        int n = read(newsockfd, filename, MAX_FILENAME_SIZE);
        if (n == 0)
        {
          close(newsockfd);
          exit(0);
        }
        FILE *fin = fopen(filename, "r");

        if (fin == NULL)
        {
          long statusmsg = -1;
          write(newsockfd, &statusmsg, sizeof(statusmsg));
        }
        else

        {
          long filesize = fsize(fin);
          write(newsockfd, &filesize, sizeof(filesize));

          char msg[filesize];
          fread(msg, sizeof(char), filesize, fin);
          fclose(fin);
          write(newsockfd, msg, sizeof(msg));
        }
      }
    }
  }

  else
  {
    printf("ready to recive commands\n");
    char command[MAX_FILENAME_SIZE];
    while (1)
    {
      scanf("%s", command);

      if (strcmp(command, "exit") == 0)
      {
        exit(0);
      }

      else if (strcmp(command, "help") == 0)
      {
        printf("available commands:\n");
        printf("exit - close programm\n");
        printf("help - show available commands\n");
      }

      else
      {
        printf("unknown command, use help to see available commands\n");
      }
    }
  }
}