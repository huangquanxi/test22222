#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

int fd;

void * handler(void * arg)
{
	struct sockaddr *p = (struct sockaddr *) arg;

	char buff[1024];
	int ret;
	int cliLen = sizeof(struct sockaddr_in);
	while(1)
	{
		memset(buff, 0, 1024);
		ret = read(0, buff, 1024);

		ret = sendto(fd, buff, ret - 1, 0, p,  cliLen);
		printf("send a msg:%s\n", buff);
	}

	return NULL;
}
int main(int argc, char ** argv)
{
	int ret;
	//int fd;

	if(argc != 2)
	{
		printf("para error\n");
		return -1;
	}

	fd = socket(AF_INET, SOCK_DGRAM, 0);// UDP   TCP:SOCK_STREAM
	if(fd < 0)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in serverAddr , clientAddr;
	int cliLen = sizeof(clientAddr);

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(fd, (struct sockaddr *)& serverAddr, sizeof(serverAddr));
	if(ret < 0)
	{
		perror("bind");
	}

	char buff[1024] = {0};
	int flag = 0;
	pthread_t pid;
	//阻塞
	while(1)
	{
		ret = recvfrom(fd, buff, 1024, 0, (struct sockaddr *) & clientAddr, &cliLen);

		printf("recv a msg:%s\n", buff);
		if(flag == 0)
		{
			ret = pthread_create(&pid, NULL, handler, &clientAddr);
			flag = 1;
		}


	}

	close(fd);
	return 0;

}
