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
	
	int i = 0;
	for(i = 0; i < 1200; ++i)
	{
		
		char buff[80];
	
		char msg[] = "Hello, tcp-server\n";
		const int msgLen = strlen(msg)+1;

		int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if(-1 == clientSocket)
		{
			perror("client socket error");
			return -1;
		}
	
		struct sockaddr_in serverAddr;
		memset(&serverAddr, 0, sizeof(serverAddr));
	
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(SERVER_PORT);/*---fix---*/
		inet_pton(AF_INET, IP_ADDRESS, (&serverAddr.sin_addr));
	
		int res = connect(clientSocket, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
		if(-1 == res)
		{
			perror("client connect error");
			return -1;
		}
		
		
		int nBytes = send(clientSocket, msg, msgLen, 0);
		if(nBytes != msgLen)
		{
			perror("client send error");
			return -1;
		}
	
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

		close(clientSocket);
	}
	return 0;
}


