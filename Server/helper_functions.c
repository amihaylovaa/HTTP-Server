#include <stdio.h>
#include <string.h>
#include <time.h>
#include "header_files/add_log_data.h"
#include "header_files/get_body.h"
#include "header_files/macros.h"
#include "header_files/get_http_request_method.h"

// get http request method from response
void GetHTTPRequestMethod(char* request, char* RequestMethod)
{
	int i;

	// find HTTP request method
	for (i = 0; request[i] != ' '; i++)
		RequestMethod[i] = request[i];

	// terminate the string containing request name
	RequestMethod[i] = '\0';
}

// create log file and fills it with data
void AddLogData(char* request, char* response)
{
	int i;
	char req[50], resp[50];
	time_t CurrentTime;
	time(&CurrentTime);
	FILE* fp;

	fp = fopen(LOG_FILE_NAME, "a+");

	for (i = 0; request[i] != '\r'; i++)
		req[i] = request[i];

	req[i] = '\0';

	for (i = 0; response[i] != '\r'; i++)
		resp[i] = response[i];

	resp[i] = '\0';

	fprintf(fp, "Request:%s\nResponse:%s\nTime:%s\n", req, resp, ctime(&CurrentTime));
	fclose(fp);
}

// get body from http request
void GetBody(char* request, char* body)
{
	int i;
	char RequestBody[BODY_SIZE * 10] = { '\0' };

	strcpy(RequestBody, strrchr(request, '\n'));

	if ((strlen(RequestBody) - 1) >= BODY_SIZE)
		strcpy(body, "Body too large");

	else
	{
		strcpy(body, RequestBody);

		for (i = 0; body[i + 1] != '\0'; i++)
			body[i] = body[i + 1];

		body[i] = '\0';
	}
}
