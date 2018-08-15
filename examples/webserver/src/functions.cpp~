
#include "allinc.h"

void* handle_client(void* h)
{
	int handle, r;
	char* pos1;
	char* pos2;
	char request[4096];

	printf("thread created\n");

	while (1)
	{
neustart:
		handle = *((int*)h);
		if (handle > -1)
		{
			printf("handle > -1!!!!!!!!!!!!!!!!!! (%d)\n", handle);
			r = read(handle, readblob, 8192);
			readblob[r] = 0;
			printf("r = %d, readblob = '%s'\n", r, readblob);
			//snprintf(writeblob, 8192, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n%s", strlen("hii"), "hii");
			//r = write(new_client, writeblob, strlen(writeblob));
			//printf("[%p] written '%s' with length %lu\n", &writeblob, writeblob, strlen(writeblob));
			//printf("r = %d, now closing connection\n", r);

			pos1 = strstr(readblob, "GET");
			printf("[%p] pos1 = '%s'\n", &pos1, pos1);

			pos1 += 4;

			pos2 = strstr(pos1, " ");
			printf("[%p] pos2 = '%s'\n", &pos2, pos2);

			memcpy(request, pos1, pos2 - pos1);
			request[pos2 - pos1] = 0;
			printf("client is requesting for '%s'\n", request);

			for (Data* d = Data::chain_first; d; d = d->chain_next)
			{
				printf("'%s' == '%s'\n", request, d->path);
				if (strcmp(request, d->path) == 0)
				{
					snprintf(writeblob, 8192, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n", d->data_len);
					write(handle, writeblob, strlen(writeblob));
					write(handle, d->data, d->data_len);
					printf("request was '%s', so sending client '%s'\n", request, writeblob);
					close(handle);

					handle = -1;
					*((int*)h) = -1;

					goto neustart;
				}
			}
			write(handle, "HTTP/1.1 404 NOT FOUND\r\nContent-Length: 298\r\n\r\n<html><head><title>ERROR - 404</title></head><body><center><h1>ERROR 404 - NOT FOUND</h1></center><h4><p>This Webserver is very sorry, but it couldn't find the requested file :-(</p><p><a href='/index.html'>go back do home</a></p></h4></body></html>", 301);
			printf("file not found - ERROR 404\n");

			close(handle);
			handle = -1;
			*((int*)h) = -1;

			printf("error 404, *h = %d\n", *((int*)h));
		}else if(handle == -2)
		{
			pthread_exit(NULL);
		}
	}
}
void listen()
{
	int debug;
	sockaddr_in addr;

	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(80);

	http_listen_handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (http_listen_handle < 0)
	{
		printf("Fail!!!: http_listen_handle = %d\n", http_listen_handle);
		exit(1);
	}

	int s = 1;
	setsockopt(http_listen_handle, SOL_SOCKET, SO_REUSEPORT, &s, sizeof(int));

	if ((debug = bind(http_listen_handle, (const sockaddr*) &addr, sizeof(sockaddr_in))) < 0)
	{
		printf("Failed to bind because %d, errno = %d\n", debug, errno);
		exit(1);
	}

	listen(http_listen_handle, 1);
	printf("http_listen_handle = %d\n", http_listen_handle);
}

void reception()
{
	fd_set fd;
	int new_client, r;
	char* pos1;
	char* pos2;
	char request[4096];

	FD_ZERO(&fd);
	FD_SET(http_listen_handle, &fd);

	if (select(http_listen_handle + 1, &fd, 0, 0, 0) > 0 && http_listen_handle > -1)
	{
		if (FD_ISSET(http_listen_handle, &fd))
		{
			new_client = accept(http_listen_handle, 0, 0);
			if (new_client < 0)
			{
				printf("accept failed, new client = %d\n", new_client);
				exit(1);
			}

			for (unsigned int i=0; i < thread_count; i++)
			{
				if (threads[i] == -1)
				{
					threads[i] = new_client;
					return;
				}
			}

			r = read(new_client, readblob, 8192);
			readblob[r] = 0;
			printf("r = %d, readblob = '%s'\n", r, readblob);
			//snprintf(writeblob, 8192, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n%s", strlen("hii"), "hii");
			//r = write(new_client, writeblob, strlen(writeblob));
			//printf("[%p] written '%s' with length %lu\n", &writeblob, writeblob, strlen(writeblob));
			//printf("r = %d, now closing connection\n", r);

			pos1 = strstr(readblob, "GET");
			printf("[%p] pos1 = '%s'\n", &pos1, pos1);

			pos1 += 4;

			pos2 = strstr(pos1, " ");
			printf("[%p] pos2 = '%s'\n", &pos2, pos2);

			memcpy(request, pos1, pos2 - pos1);
			request[pos2 - pos1] = 0;
			printf("client is requesting for '%s'\n", request);

			for (Data* d = Data::chain_first; d; d = d->chain_next)
			{
				printf("'%s' == '%s'\n", request, d->path);
				if (strcmp(request, d->path) == 0)
				{
					snprintf(writeblob, 8192, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n", d->data_len);
					write(new_client, writeblob, strlen(writeblob));
					write(new_client, d->data, d->data_len);
					printf("request was '%s', so sending client '%s'\n", request, writeblob);
					close(new_client);
				}
			}
			write(new_client, "HTTP/1.1 404 NOT FOUND\r\nContent-Length: 298\r\n\r\n<html><head><title>ERROR - 404</title></head><body><center><h1>ERROR 404 - NOT FOUND</h1></center><h4><p>This Webserver is very sorry, but it couldn't find the requested file :-(</p><p><a href='/index.html'>go back do home</a></p></h4></body></html>", 301);
			printf("file not found - ERROR 404\n");

			close(new_client);
		}
	}
}

void GetAllWebsites(char* path, int dirlen) /* data Directory durchgehen und alle Webseiten suchen und dann auch den Spezial-Kerzeenverkauf Ordner einbinden */
{
	DIR* dir;
	struct dirent* files;
	Data* d;
	char loadpath[4096];

	dir = opendir(path);
	if (dir == NULL)
	{
		printf("Could not open dir '%s'\n", path);
		exit(1);
	}

	while ((files = readdir(dir)) != NULL)
	{
		printf("read file %s, it %s a directory\n", files->d_name, files->d_type == 4 ? "is" : "isn't");
		if (files->d_type == 8)
		{
			d = new Data();
			snprintf(loadpath, 4096, "%s/%s", path, files->d_name);
			d->Load(loadpath, dirlen);
			printf("loaded %s, its content ist '%s'\n", loadpath, d->data);
		}else if (files->d_type == 4 && strcmp("..", files->d_name) < -1 && strcmp(".", files->d_name) < -1)
		{
			printf("--------------------------- directory found, going in because of recursive search ----------------------\n");
			snprintf(loadpath, 4096, "%s/%s", path, files->d_name);
			GetAllWebsites(loadpath, dirlen);
		}
	}
}
