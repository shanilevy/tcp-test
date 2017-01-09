/*********************************************
Author : Eran Blumenthal
Creation Date : 02-01-2014
Modification Date : 06-01-2014
Description : 
**********************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string.h>
#include <stdlib.h>

#include "clientserver.h"


int main(int argc, char *argv[])
{
	
	char buff[80];
	char msgToClient[] = "Welcome to tcp_server\n";

	const int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == listeningSocket)
	{
		perror("server listeningSocket error");
		return -1;
	}
	
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, IP_ADDRESS, (&serverAddr.sin_addr));

	int serverAddrLen = sizeof(serverAddr);
	
	int rval_Bind = bind(listeningSocket, (const struct sockaddr*)&serverAddr, sizeof(serverAddr) );
	if(-1 == rval_Bind)
	{
		perror("server bind error");
		return -1;
	}
	int optval = 1;
	setsockopt(listeningSocket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		
	int res = listen(listeningSocket, 500);
	if(-1 == res)
	{
		perror("server bind error");
		return -1;
	}

	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(SERVER_PORT);
	
	inet_pton(AF_INET, IP_ADDRESS, (&clientAddr.sin_addr));

	int clientAddrLen = sizeof(clientAddr);
	
	while(1)
	{
		int clientSocket = accept(listeningSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
		
		int recvBytes = recv(clientSocket, buff, sizeof(buff), 0);
		if(recvBytes < 1)
		{
			if(recvBytes == 0)
			{
				perror("client recv error (no server)");
			}
			else
			{
			perror("client recv error");
			}
			close(clientSocket);
			return -1;
		}

		int sentBytes = sendto(clientSocket, msgToClient, sizeof(msgToClient), 0, (const struct sockaddr*)&clientAddr, sizeof(clientAddr));
		if(-1 == sentBytes)
		{
			perror("server sendto error");
		}
	}

	close(listeningSocket);
	return 0;
}

