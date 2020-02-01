#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include "header_files/get_http_request_method.h"
#include "header_files/macros.h"
#include "header_files/get_body.h"
#include "header_files/get_request.h"
#include "header_files/post_request.h"
#include "header_files/put_request.h"
#include "header_files/delete_request.h"
#include "header_files/add_log_data.h"

#pragma comment(lib,"ws2_32.lib")

void main()
{
	// initialize socket's library
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) == -1)
	{
		printf("Failed initialization");
		return;
	}

	char request[REQUEST_SIZE] = {'\0'};
	char response[RESPONSE_SIZE] = {'\0'};
	char RequestMethod[REQUEST_METHOD_SIZE];
	struct sockaddr_in ClientAddress;
	struct sockaddr_in ServerAddress;
	int size = sizeof(ServerAddress);
	int ServerSocketDescriptor;
	int ConnectedSocketDescriptor;
	
	// create server socket descriptor
	if ((ServerSocketDescriptor = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Receiving server socket descriptor failed");
		return;
	}

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(1221);
	ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	// bind address and port to the server socket
	if (bind(ServerSocketDescriptor, (struct sockaddr*)&ServerAddress, size) == -1)
	{
		printf("Binding Failed");
		return;
	}

	// listen for client requets
	if (listen(ServerSocketDescriptor, 128) != 0)
	{
		printf("Listening Failed");
		return;
	}

	while (1)
	{
		// accept client request
		if ((ConnectedSocketDescriptor = accept(ServerSocketDescriptor, (struct sockaddr*) & ClientAddress, &size)) == -1)
		{
			printf("Accepting client request failed ");
			return;
		}

		// receive request
		if (recv(ConnectedSocketDescriptor, request, sizeof(request), 0) == -1)
		{
			printf("Client request is not received");
			return;
		}

		// get http request method from the request
		GetHTTPRequestMethod(request, RequestMethod);

		// http requests' handling
		if (strcmp(RequestMethod, "GET") == 0)
			GetRequest(ConnectedSocketDescriptor, request, response);

		else if (strcmp(RequestMethod, "POST") == 0)
			PostRequest(ConnectedSocketDescriptor, request, response);

		else if (strcmp(RequestMethod, "PUT") == 0)
			PutRequest(ConnectedSocketDescriptor, request, response);

		else if (strcmp(RequestMethod, "DELETE") == 0)
			DeleteRequest(ConnectedSocketDescriptor, request, response);

		// if the request is not supported by the server
		else
		{
			strcpy(response, "HTTP/ 1.1 405 Method Not Allowed \r\n\r\nThe server does not support this request \r\n\r\n");
			send(ConnectedSocketDescriptor, response, strlen(response) + 1, 0);
		}

		AddLogData(request, response);

		memset(request, '\0', strlen(request));
		memset(response, '\0', strlen(response));

		closesocket(ConnectedSocketDescriptor);
	}

	WSACleanup();
}