#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void fill_random_nums(int *nums, int n, int min, int max)
{
  srand((unsigned)(time(0)));
  for (int i = 0; i < n; i++)
  {
    nums[i] = min + rand() % (max - min + 1);
  }
}

void print_nums(int *nums, int c)
{
  for (int i = 0; i < c; i++)
  {
    printf("%d ", nums[i]);
  }
  printf("\n");
}

long timedifference(struct timeval t0, struct timeval t1)
{
  return (t1.tv_sec - t0.tv_sec) * 1000000 + (t1.tv_usec - t0.tv_usec);
}

int main(int argc, char *argv[])
{
  int n = 0, min, max;
  while (n <= 0)
  {
    printf("amount of numbers: ");
    scanf("%d", &n);
  }
  printf("min: ");
  scanf("%d", &min);
  printf("max: ");
  scanf("%d", &max);

  int sockfd;
  char sendline[n * sizeof(int)], recvline[n * sizeof(int)];
  struct sockaddr_in servaddr;

  sockfd = socket(PF_INET, SOCK_STREAM, 0);

  char *sep = strchr(argv[1], ':');
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(sep + 1));
  sep[0] = 0;
  inet_aton(argv[1], &servaddr.sin_addr);
  connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  fill_random_nums((int *)sendline, n, min, max);
  print_nums((int *)sendline, n);

  struct timeval start, end;
  write(sockfd, &n, sizeof(int));
  write(sockfd, sendline, sizeof(sendline));
  gettimeofday(&start, 0);
  int n1 = read(sockfd, recvline, sizeof(recvline));
  gettimeofday(&end, 0);
  print_nums((int *)recvline, n);
  printf("response time: %ldns\n", timedifference(start, end));
  close(sockfd);
}