
#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>

void Sort(int data[], int lenD)
{
  int j = 0;
  int tmp = 0;
  for(int i=0; i<lenD; i++){
    j = i;
    for(int k = i; k<lenD; k++){
      if(data[j]>data[k]){
        j = k;
      }
    }
    tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
  }
}

int main(int argv, char *argc[])
{
if (argv <= 1)
{
	printf("not enough params\n");
	return -1;
}

char *paramStr = argc[1];
int memId = atoi(paramStr);

if (memId == 0)
{
	printf("incorrect parameter string: %s\n", paramStr);
	return -2;
}

printf("receivning the memory data: shmid = %i\n", memId);

int *mem = (int *)shmat(memId, NULL, 0);
if (NULL == mem)
{
	printf("error with shmat()\n");
	return -3;
}

for(int i = 0; i < 20; i++)
{
	printf("%d\n",mem[i] );
}

printf("Sorted array:\n" );


Sort(mem, 20);

for(int i = 0; i < 20; i++)
{
	printf("%d\n",mem[i] );
}
return 0;
}
