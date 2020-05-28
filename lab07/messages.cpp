#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <iostream>
#include <time.h>

using namespace std;

int comp(const void *a, const void *b)
{
    return (*((int *)a) - *((int *)b));
}

void swap(int *a, int i, int j)
{
  int s = a[i];
  a[i] = a[j];
  a[j] = s;
}

bool NextSet(int *a)
{
  int j = 2;
  while (j != -1 && a[j] >= a[j + 1]) j--;
  if (j == -1)
    return false; 
  int k = 3;
  while (a[j] >= a[k]) k--;
  swap(a, j, k);
  int l = j + 1, r = 3; 
  while (l<r)
    swap(a, l++, r--);
	return true;
}

struct message
{
    long mtype;
    int mas[4];
}mess;

void* comb(void* idm)
{
	int id = *((int*)idm);
	struct message thr;
	msgrcv(id, &thr, 16, 5, 0);
	qsort(thr.mas,4, sizeof(int),comp);
	thr.mtype = 6;
		msgsnd(id, &thr, 16, 0);
	while (NextSet(thr.mas))
	{
		thr.mtype = 6;
		msgsnd(id, &thr, 16, 0);
	}
	thr.mtype = 6;
	thr.mas[0] = -1;
	msgsnd(id, &thr, 16, 0);

}

int main()
{
	int iter = 0;
	int msgid = msgget(IPC_PRIVATE, 0666|IPC_CREAT);
	if (msgid < 0)
	{
		cout << "error with msgget." << endl;
		exit(-1);
	}
	srand(time(NULL));
	cout << "Source: ";
	for (int i = 0; i < 4; i++)
	{

		mess.mas[i] = rand()%100;
		cout << mess.mas[i] << "; ";
	} cout << endl;

	mess.mtype = 5;
	msgsnd(msgid, &mess, 16, 0);
	pthread_t thread0;
	pthread_create(&thread0, NULL, comb, (void*)&msgid);
	pthread_join(thread0, NULL);

	while(1)
	{
		msgrcv(msgid, &mess, 16, 6, 0);
		if(mess.mas[0] == -1)
			{ break;}
		else
		{
			for(int i = 0; i < 4; i++)
			{
				cout << mess.mas[i] << " ";
			}
			iter++;
			cout << endl;
		}
	}

	cout << "All combinations: " << iter << endl;

	msgctl(msgid, IPC_RMID, NULL);

	return 0;
}