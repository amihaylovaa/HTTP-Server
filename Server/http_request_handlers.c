#include <stdio.h>
#include <string.h>
#include <winsock.h>
#include  "header_files/put_request.h"
#include "header_files/post_request.h"
#include "header_files/get_Request.h"
#include "header_files/delete_request.h"
#include "header_files/macros.h"
#include "header_files/get_body.h"


void PostRequest(int ConnectedSocketDescriptor, char* request, char* response)
{
	char body[BODY_SIZE];
	char FileContent[BODY_SIZE];
	FILE* fp;

	fp = fopen(FILE_NAME, "w");

	// get request's body
	GetBody(request, body);

	// prevent overflow
	if (strcmp(body, "Body too large") == 0)
	{
		strcpy(body, BODY_TOO_LARGE);
		sprintf(response, "HTTP/1.1 413 Request Entity Too Large\r\nContent-type: text/html;charset=utf-8\r\n\r\n%s\r\n\r\n", body);
	}

	// check if the body is empty and form appropriate response
	else if (strlen(body) == 0)
		sprintf(response, "%s", NO_CONTENT_RESPONSE);

	// form appropriate response for successfully created resource
	else
	{

		fprintf(fp, "<html><body> %s </body></html>", body);
		rewind(fp);
		fgets(FileContent, BODY_SIZE, fp);
		fclose(fp);

		sprintf(response, "HTTP/1.1 200 OK\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", FileContent);
	}

	send(ConnectedSocketDescriptor, response, strlen(response) + 1, 0);
}

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

void PutRequest(int ConnectedSocketDescriptor, char* request, char* response)
{
	char body[BODY_SIZE];
	char FileContent[BODY_SIZE];

	// get request's body
	GetBody(request, body);

	//prevent overflow
	if (strcmp(body, "Body too large") == 0)
	{
		strcpy(body, BODY_TOO_LARGE);
		sprintf(response, "HTTP/1.1 413 Request Entity Too Large\r\ncontent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", body);
	}

	// check if the body is empty and form appropriate response
	else if (strlen(body) == 0)
		strcpy(response, NO_CONTENT_RESPONSE);

	// update existing resource or create new one
	else
	{
		FILE* fp;
		fp = fopen(FILE_NAME, "w+");

		fprintf(fp, "<html><body> %s </body></html>", body);
		rewind(fp);
		fgets(FileContent, BODY_SIZE, fp);
		fclose(fp);

		sprintf(response, "HTTP/1.1 200 OK\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", FileContent);
	}

	send(ConnectedSocketDescriptor, response, strlen(response) + 1, 0);
}

void DeleteRequest(int ConnectedSocketDescriptor, char* request, char* response)
{
	char body[BODY_SIZE];
	FILE* fp;

	fp = fopen(FILE_NAME, "r");

	// get request's body
	GetBody(request, body);

	// check if body is empty and form appropriate response
	if (strlen(body) == 0)
		strcpy(response, NO_CONTENT_RESPONSE);

	//prevent overflow
	else if (strcmp(body, "Body too large") == 0)
	{
		strcpy(body, BODY_TOO_LARGE);
		sprintf(response, "HTTP/1.1 413 Request Entity Too Large\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", body);
	}

	// if the resource does not exist, form appropriate response
	else if (fp == NULL)
	{
		strcpy(body, NOT_FOUND);
		sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", body);
	}

	// delete resource and form appropriate response
	else if (strcmp(FILE_NAME, body) == 0)
	{
		fclose(fp);

		if (remove(FILE_NAME) == 0)
		{
			strcpy(body, "<html><body> Sucessfully deleted resource </body></html>");
			sprintf(response, "HTTP/1.1 200 OK\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", body);
		}
	}

	// resource not found
	else
	{
		fclose(fp);

		strcpy(body, "<html><body>404 Not Found <br> </br> Resource does not exist</body></html>");
		sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-type: text/html; charset=utf-8\r\n\r\n%s\r\n\r\n", body);
	}

	send(ConnectedSocketDescriptor, response, strlen(response) + 1, 0);
}