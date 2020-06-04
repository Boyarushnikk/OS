#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

void game(int listenfd, int cl1, int cl2)
{
	while (1)
	{
		int num = 8;
		srand(time(NULL));
		rand();
		int rev[8];
		int check;
		for (int i = 0; i < num; i++)
		{
			rev[i] = 0;
		}
		rev[rand() % num] = 1;
		for (int i = 0; i < num; i++)
		{
			if (i % 2 == 0) // ход 1 игрока
			{
				check = 4;
				check = htons(check);
				write(cl1, &check, sizeof(int));
				read(cl1, &check, sizeof(int));
				check = ntohs(check);
				if (check == 0) // ход
				{
					if (rev[i] == 0) // успешный ход
					{
						check = 0;
						check = htons(check);
						write(cl1, &check, sizeof(int));
					}
					else // смэрть
					{
						check = 1;
						check = htons(check);
						write(cl1, &check, sizeof(int));
						check = 3;
						check = htons(check);
						write(cl2, &check, sizeof(int));
						break;
					}
				}
				else // сдался
				{
					check = 2;
					check = htons(check);
					write(cl2, &check, sizeof(int));
					cl1 = accept(listenfd, (struct sockaddr *)NULL, NULL); //ждем подключеня 1 клиента
					break;
				}
			}
			else // аналогично для хода второго игрока
			{
				check = 4;
				check = htons(check);
				write(cl2, &check, sizeof(int));
				read(cl2, &check, sizeof(int));
				check = ntohs(check);
				if (check == 0)
				{
					if (rev[i] == 0)
					{
						check = 0;
						check = htons(check);
						write(cl2, &check, sizeof(int));
					}
					else
					{
						check = 1;
						check = htons(check);
						write(cl2, &check, sizeof(int));
						check = 3;
						check = htons(check);
						write(cl1, &check, sizeof(int));
						break;
					}
				}
				else
				{
					check = 2;
					check = htons(check);
					write(cl1, &check, sizeof(int));
					cl2 = accept(listenfd, (struct sockaddr *)NULL, NULL); //ждем подключеня 2 клиента
					break;
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int listenfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[1025];
	time_t ticks;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 10);
	int cl1 = accept(listenfd, (struct sockaddr *)NULL, NULL); //ждем подключеня 1 клиента
	int cl2 = accept(listenfd, (struct sockaddr *)NULL, NULL); //ждем подключеня 2 клиента
	game(listenfd, cl1, cl2);
}
