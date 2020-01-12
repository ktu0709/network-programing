#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	clock_t start, end;
	FILE* fp;
	WSADATA wsaData;
	SOCKET hServSock;
	char message[BUF_SIZE];
	char fin[] = { 0 };
	size_t rtn;
	SOCKADDR_IN servAddr, clntAddr;
	int szClntAddr;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	fopen_s(&fp, "copy.txt", "w");
	if (fp == NULL) {
		ErrorHandling("fopen() error");
		exit(1);
	}

	hServSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));
	memset(&clntAddr, 0, sizeof(clntAddr));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	start = clock();
	while (1) {
		memset(message, 0, sizeof(message));
		szClntAddr = sizeof(clntAddr);
		rtn = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &szClntAddr);
		printf("rtn: %d\n", rtn);
		if (rtn == -1)
			ErrorHandling("sendto() error");
		else if (rtn == 0)
			break;
		else
			fwrite(message, rtn, 1, fp);
	}
	end = clock();
	printf("time: %.3f", (float)(end - start) / CLOCKS_PER_SEC);

	closesocket(hServSock);
	fclose(fp);
	WSACleanup();

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
