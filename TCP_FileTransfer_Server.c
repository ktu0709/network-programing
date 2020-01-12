#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	FILE* fp;
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	size_t strlen;
	int  i, rtn;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	fopen_s(&fp, "test.txt", "r");
	if (fp == NULL) {
		ErrorHandling("fopen() error");
		exit(1);
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

		if (listen(hServSock, 5) == SOCKET_ERROR)
			ErrorHandling("listen() error");

			szClntAddr = sizeof(clntAddr);
	for (i = 0; i < 5; i++) {
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
		if (hClntSock == INVALID_SOCKET) {
			ErrorHandling("accept() error");
		}
		else {
			printf("Connected client %d \n", i + 1);
		}

		while (!feof(fp)) {
			memset(message, sizeof(message), 0);
			strlen = fread_s(message, sizeof(message), 1, BUF_SIZE, fp);
			rtn = send(hClntSock, message, strlen, 0);
			if (rtn == -1)
				printf("send()1 error!");

		}
		closesocket(hClntSock);
	}
	fclose(fp);
	closesocket(hServSock);
	WSACleanup();

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
