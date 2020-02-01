#include <stdio.h>
#include <winsock.h>
#include "header_files/get_Request.h"
#include "header_files/macros.h"

void GetRequest(int ConnectedSocketDescriptor, char* request, char* response)
{
	char body[BODY_SIZE];
	FILE* fp;

	fp = fopen(FILE_NAME, "r");

	// form appropriate response if the resource does not exist
	if (fp == NULL)
	{
		strcpy(body, NOT_FOUND);
		sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", body);
	}

	// read resource content and form appropriate response
	else
	{
		fgets(body, BODY_SIZE, fp);
		sprintf(response, "HTTP/1.1 200 OK\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", body);
		fclose(fp);
	}

	send(ConnectedSocketDescriptor, response, strlen(response) + 1, 0);
}