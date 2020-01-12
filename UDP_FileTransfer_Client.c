#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	FILE* fp;
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	size_t str_len, rtn;
	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	fopen_s(&fp, "test.txt", "r");
	if (fp == NULL) {
		ErrorHandling("fopen() error");
		exit(1);
	}

	hSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	fputs("Recive text file......\n", stdout);
	while (1) {
		memset(message, 0, sizeof(message));
		rtn = fread(message, 1, BUF_SIZE, fp);
		printf("%s\n", message);
		str_len = send(hSocket, message, rtn, 0);
		if (str_len == -1)
			ErrorHandling("send() error!");
		if (feof(fp) != 0)
			break;
	}
	printf("\nRecive text file done");

	closesocket(hSocket);
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
