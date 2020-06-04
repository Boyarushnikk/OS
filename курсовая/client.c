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

int main(int argc, char *argv[])
{
	int sockfd = 0, n = 0;
	char recvBuff[1024];
	struct sockaddr_in serv_addr;
	if (argc != 3)
	{
		printf("\n Usage: %s <ip of server> \n", argv[0]);
		return 1;
	}
	memset(recvBuff, '0', sizeof(recvBuff));
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	}
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // подключение
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	}
	printf("Connection sucsess\n");
	printf("Wait..\n");
	while (1)
	{
		int check;
		read(sockfd, &check, sizeof(int)); // Читаем код от сервера
		check = ntohs(check);
		switch (check)
		{
		case 0:
			printf("Good\n"); // успешный ход
			break;

		case 1:
			printf("Dead\n"); // смэрть
			break;

		case 2:
			printf("Opponent give up\n"); // Противник сдался
			printf("Wait..\n");
			break;

		case 3:
			printf("Opponent is dead\n"); // Противник умер
			break;

		case 4: // Наш ход
			printf("Choose: 0-shoot, 1-out\n");
			do
			{
				scanf("%d", &check);
			} while (check != 0 && check != 1); // Проверка ввода 0 или 1
			if (check == 0)
			{
				check = htons(check);
				write(sockfd, &check, sizeof(int)); // отправляем ход
			}
			else
			{
				check = htons(check);
				write(sockfd, &check, sizeof(int)); // отправляем ход и завершаем работу
				return 0;
			}
			break;
		}
	}
}
