#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	clock_t start, end;
	FILE* fp;

	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	size_t str_len;
	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	fopen_s(&fp, "copy.txt", "w");
	if (fp == NULL) {
		ErrorHandling("fopen() error");
		exit(1);
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		fputs("Connected...........\n", stdout);

	fputs("Recive text file......\n", stdout);
	start = clock();
	while (1) {
		str_len = recv(hSocket, message, BUF_SIZE, 0);
		if (str_len == 0) {
			break;
		}
		fwrite(message, str_len, 1, fp);
	}
	end = clock();
	printf("Recive text file done \ntime: %.3f\n",(float)(end-start)/CLOCKS_PER_SEC);

	fclose(fp);
	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
