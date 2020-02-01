#include <stdio.h>
#include <winsock.h>
#include "header_files/post_request.h"
#include "header_files/macros.h"
#include "header_files/get_body.h"

void PostRequest(int ConnectedSocketDescriptor, char* request, char* response)
{
	char body[BODY_SIZE];
	char FileContent[BODY_SIZE];
	FILE* fp;

	fp = fopen(FILE_NAME, "w+");

	// get request's body
	GetBody(request, body);

	//prevent overflow
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