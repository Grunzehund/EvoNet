
#include "allinc.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("syntax is: ./webserver $(count of threads)\n");
		exit(1);
	}
	thread_count = atoi(argv[1]);

	handles = new int[thread_count];
	threads = new pthread_t[thread_count];

	for (unsigned int i=0; i < thread_count; i++)
	{
		handles[i] = -1;
		pthread_create(&threads[i], NULL, handle_client, &handles[i]);
		pthread_kill(&threads[i], SIGSTOP);
	}
	GetAllWebsites("./data", 6);

	listen();
	printf("listen_handle = %d\n", http_listen_handle);

	while (1)
		reception();

	return 0;
}
