#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

int main()
{
	const size_t memSize = 20*sizeof(int);

	int arr[20];
	for (int i = 0; i < 20; i++)
	{
		arr[i] = rand()%100;
	}
	
	int memId = shmget(IPC_PRIVATE, memSize, 0600|IPC_CREAT|IPC_EXCL);

	printf("shmid = %i\n" , memId);
	if (memId <= 0)
	{ 
		printf("error with shmget()\n");
		return -1;
	}


	int *mem = (int*)shmat(memId, NULL,0);
	if (NULL == mem)
	{
		printf("error with shmat()\n");
		return -2;
	}

	
	for (int i = 0; i < 20; i++)
	{
		mem[i]=arr[i];
	}

	char callbuf[1024];
	sprintf(callbuf, "./receiver %i", memId);
	system(callbuf);

	return 0;
}
