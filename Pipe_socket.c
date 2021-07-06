#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define FIFO1 "/tmp/fifo.1"
#define MAXLINE 80

FILE *fp;

void DieWithError(char *errorMessage)
{
	perror(errorMessage);
	exit(1);
}
int doit(void)
{
	pid_t pid;
/*	int status = 0;
	FILE *readfd;
	FILE *writefp;
	char buff[MAXLINE] =
	{ '\0' };
	char str[MAXLINE] =
	{ '\0' };
	ssize_t len, m;
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[1025];
	char recvBuff[1025];
	time_t ticks;
	int sockfd = 0;
	int n = 0;
	//char recvBuff[1024];

	mkfifo(FIFO1, 0666);*/
	pid = fork();
	if (-1 == pid)
	{
		perror("fork"); /* произошла ошибка */
		exit(1); /*выход из родительского процесса*/
	}
	else if (0 == pid)
	{
		int sockfd = 0, n = 0;
		char recvBuff[1024];
		struct sockaddr_in serv_addr;
		memset(recvBuff, '0', sizeof(recvBuff));
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("\n Error : Could not create socket \n");
			return 1;
		}
		memset(&serv_addr, '0', sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(5000);
		if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
		{
			printf("\n inet_pton error occured\n");
			return 1;
		}
		if (connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))
				< 0)
		{
			printf("\n Error : Connect Failed \n");
			return 1;
		}

		while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0)
		{
			recvBuff[n] = 0;
			if (fputs(recvBuff, stdout) == EOF)
			{
				printf("\n Error : Fputs error\n");
			}
		}

		if (n < 0)
		{
			printf("\n Read error \n");
		}
	}
	else
	{
		int listenfd = 0, connfd = 0;
		struct sockaddr_in serv_addr;
		char sendBuff[1025];
		time_t ticks;

		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		memset(&serv_addr, '0', sizeof(serv_addr));
		memset(sendBuff, '0', sizeof(sendBuff));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(5000);
		bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
		listen(listenfd, 10);

		while (1)
		{
			connfd = accept(listenfd, (struct sockaddr*) NULL, NULL);
			ticks = time(NULL);
			snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
			write(connfd, sendBuff, strlen(sendBuff));
			close(connfd);
			sleep(1);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	doit();
	return 0;
}
