#define REQUEST_SIZE 4096
#define RESPONSE_SIZE 4096
#define BODY_SIZE 2048
#define DELIMITER_SIZE 16
#define REQUEST_METHOD_SIZE 10
#define FILE_NAME "index.html"
#define LOG_FILE_NAME "Log.log"
#define NO_CONTENT_RESPONSE "HTTP/1.1 204 No Content\r\nContent-type: text/html;charset=utf-8\r\n\r\n"
#define BODY_TOO_LARGE "<html><body>413 Request Entity Too Large <br> </br> Body is too large to be proceeded</body></html>"
#define NOT_FOUND "<html> <body> 404 Not Found <br></br> Resource does not exist </body> </html>"

