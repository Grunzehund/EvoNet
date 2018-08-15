#define WINVER 0x0501
#include <kui/kui.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#undef DrawText

#define PUBLIC_KEY      1231
#define MAX_INPUT_FIELD 6000

#define INPUT_MODE_MIXKEY  1
#define INPUT_MODE_MESSAGE 2
#define INPUT_MODE_DECRYPT 3
#define INPUT_MODE_CONNECT 4


int quit;

Kui::Font font;
int handle;
char ip[15];

void OnInit()
{
	handle = 0;
}
bool prank()
{
	for(int i=0; i < 15; i++)
		ip[i] = rand()%9 + '1';
	ip[3] = '.';
	ip[7] = '.';
	ip[11] = '.';
	printf("The next ip is %s\n", ip);

	addrinfo* result;
	sockaddr_in addr;

	result = 0;

	if (getaddrinfo(ip, 0, 0, &result) || !result)
		return false;


	memset(&addr, 0, sizeof(sockaddr_in)); /* alles auf 0 setzen */
	addr.sin_family = AF_INET;  /* Internet TCP/IP */
	memcpy((char *)&addr.sin_addr.s_addr, (char *) &((struct sockaddr_in *) result->ai_addr)->sin_addr.s_addr,4);
	addr.sin_port = htons(1337); 

	handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	connect(handle, (struct sockaddr*)&addr, sizeof(addr));
	close(handle);
}
void OnKeyDown(int key)
{
	switch(key)
	{
		case Kui::KEY_ESC:
			quit = 1;
			break;
	}
}
void OnPaint()
{
	Kui::Gc gc;
	gc.SetColor(0xFFFFFF);
	gc.Erase();

	gc.SetColor(0);
	gc.SetFont(font);
	gc.DrawText(100, 100, ip);

	Kui::SwapBuffers();
}
void OnTimer()
{
	prank();
}

/**********************************************************************************************************************************************************/
/* Hauptfunktion                                                                                                                                         */
/********************************************************************************************************************************************************/




int main(int argc, char** argv)
{
	if(argc < 1)
		return 1;
	Kui::Event e;

	srand(time(0));
	//Kui::SetFullScreen(true);
	//Kui::SetWindowSize(3700, 2130);
	//Kui::SetWindowSize(3700, 2130);
	// Kui::SetWindowSize(1800, 1000);

	Kui::Start();

	font.Load(Kui::FONT_SANS, 30);
	OnInit();

	quit = 0;


	while (quit != 1)
	{
		switch (e.Next())
		{
			case Kui::EVENT_IDLE:
				prank();
				OnPaint(); /* Bildschirm malen */
				break;

			case Kui::EVENT_KEYDOWN:
				OnKeyDown(e.GetKey());
				break;

			case Kui::EVENT_TIMER:
				OnTimer();
				break;
		}
	}

	Kui::Stop();
	return 1;
}

//bool network_connect()
//{
//	addrinfo* result;
//	sockaddr_in addr;
//
//	result = 0;
//	if (getaddrinfo(ip_address_field, 0, 0, &result) || !result)
//		return false;
//
//
//	memset(&addr, 0, sizeof(sockaddr_in)); /* alles auf 0 setzen */
//	addr.sin_family = AF_INET;  /* Internet TCP/IP */
//	memcpy((char *)&addr.sin_addr.s_addr, (char *) &((struct sockaddr_in *) result->ai_addr)->sin_addr.s_addr,4);
//	addr.sin_port = htons(1337); 
//
//
//	network_connected_handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (connect(network_connected_handle, (struct sockaddr*)&addr, sizeof(addr)))
//	{
//		close(network_connected_handle);
//		network_connected_handle = -1;
//		freeaddrinfo(result);
//		return false;
//	}
//
//	freeaddrinfo(result);
//	return true;
//}
//
//
//
//
//
//
//void network_send_message()
//{
//	char *input_field_current;
//	int lenth_todo;
//	int written;
//
//	input_field_current = input_field;
//	lenth_todo = cursor;
//
//	while (lenth_todo > 0)
//	{
//		written = write(network_connected_handle, input_field_current, lenth_todo);
//		if (written < 0)
//		{
//			close(network_connected_handle);
//			network_connected_handle = -1;
//			return;
//		}
//		lenth_todo -= written;
//		input_field_current += written;
//	}
//}
//
//
//
//
//void network_reception()
//{
//	fd_set fd;
//	int max, r;
//	timeval t;
//
//	if (network_listen_handle < 0)
//		return;
//
//	FD_ZERO(&fd);
//	FD_SET(network_listen_handle, &fd);
//	max = network_listen_handle;
//	if (network_connected_handle >= 0)
//	{
//		FD_SET(network_connected_handle, &fd);
//		if (max < network_connected_handle)
//			max = network_connected_handle;
//	}
//
//	t.tv_sec = 0;
//	t.tv_usec = 0;
//	if (select(max + 1, &fd, 0, 0, &t) > 0)
//	{
//		if (FD_ISSET(network_listen_handle, &fd))
//		{
//			/* TODO jemand ruft an; jemand will sich connecten -> accept() */
//			if(network_connected_handle>=0)
//				close(network_connected_handle);
//         		network_connected_handle=accept(network_listen_handle, 0, 0);
//         		input_mode=INPUT_MODE_MESSAGE;
//		}
//
//		if (FD_ISSET(network_connected_handle, &fd))
//		{
//			// TODO
//			r = read(network_connected_handle, data_from_network, MAX_INPUT_FIELD);
//			if(r<0)
//			{
//				memcpy(data_from_network,"disconnected", 13);
//				close(network_connected_handle);
//				network_connected_handle=-1;
//			}
//			else
//				data_from_network[r]=0;
//		}
//	}
//}
