#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* print_message_function1(void *ptr)
{
	char *message = (char *)ptr;
	for (int i = 1; i <= 10; i++)
	{
		printf("%s (%d)\n", message,i);
		sleep(1);
	}

}
void* print_message_function2(void *ptr)
{
	char *message = (char *)ptr;
		for (int i = 1; i <= 12; i++)
	{
		printf("%s %d\n", message,i);
		sleep(2);
	}
}

int main()
{
	pthread_t thread1, thread2;
	const char *message1 = "Hello Threads ";
	const char *message2 = "This is iteration ";
	int res1 = pthread_create(&thread1, NULL, print_message_function1,
(void*)message1);
	int res2 = pthread_create(&thread2, NULL, print_message_function2,
(void*)message2); 
	
	pthread_join(thread1, (void **)NULL);
	pthread_join(thread2, (void **)NULL); 

	return 0;
}
