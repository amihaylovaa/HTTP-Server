Simple HTTP server that handles GET, POST, PUT and DELETE requests with static content.

Project's basic functionality is built on creating socket,
binding it to an address and port, listening and accepting client requests.

The server is running on address 127.0.0.1 and port 1221.

The following HTTP methods are handled : 

GET - extract static content from file 
-
Responses :
 - 404 Not Found - if the file does not exist
 - 200 OK - if file exists and its content is returned

POST - creates an html file with content received from request's body as raw text
-
Responses :
*  413 Request Entity Too Large - if the body is too large to be processed 
* 204 No Content - if body is empty
* 200 OK  -  successfully added content to the file 

PUT - updates file's content or create a file with a content received from request's body as raw text
-
Responses:
*  413 Request Entity Too Large - if the body is too large to be processed
* 204 No Content - if body is empty
* 200 OK - successfully added content to the file or a created new one.

DELETE - delete file by its name received from body's request as raw text
-
Responses:
*  404 Not Found - if the file does not exist
*  413 Request Entity Too Large - if the body is too large to be processed t
* 204 No Content - if body is empty
* 200 OK -  successfully deleted file.

Each request and its response are logged in log file with a corresponding timestamp.
