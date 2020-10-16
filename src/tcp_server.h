#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TCP_SERVER_DEFAULT_PORT "8083"
#define TCP_SERVER_BAD_SOCKET -1
#define TCP_SERVER_BACKLOG 10
#define TCP_SERVER_ERROR_MESSAGE "error"

// Contains all of the information needed to create to connect to the server and
// send it a message.
typedef struct Config {
    char *port;
} Config;

typedef struct Request {
    char *action;
    char *message;
} Request;

typedef struct Response {
    char *message;
} Response;

// Parses the options given to the program. It will return a Config struct with the necessary
// information filled in. argc and argv are provided by main. If an error occurs in processing the
// arguments and options (such as an invalid option), this function will print the correct message
// and then exit.
Config tcp_server_parse_arguments(int argc, char *argv[]);

////////////////////////////////////////////////////
///////////// SOCKET RELATED FUNCTIONS /////////////
////////////////////////////////////////////////////

// Create and bind to a server socket using the provided configuration. A socket file descriptor
// should be returned. If something fails, a -1 must be returned.
int tcp_server_create(Config config);

// Listen on the provided server socket for incoming clients. When a client connects, return the
// client socket file descriptor. This is a blocking call. If an error occurs, return a -1.
int tcp_server_accept(int socket);

// Read data from the provided client socket, parse the data, and return a Request struct. This
// function will allocate the necessary buffers to fill in the Request struct. The buffers contained
// in the Request struct must be freed using tcp_server_client_cleanup. If an
// error occurs, return an empty request and this function will free any allocated resources.
Request tcp_server_receive_request(int socket);

// Sends the provided Response struct on the provided client socket. Return 1 (EXIT_FAILURE) if an
// error occurs, otherwise return 0 (EXIT_SUCCESS).
int tcp_server_send_response(int socket, Response response);

// Closes the provided client socket and cleans up allocated resources.
void tcp_server_client_cleanup(int socket, Request request, Response response);

// Closes provided server socket
void tcp_server_cleanup(int socket);

////////////////////////////////////////////////////
//////////// PROTOCOL RELATED FUNCTIONS ////////////
////////////////////////////////////////////////////

// Convert a Request struct into a Response struct. This function will allocate the necessary
// buffers to fill in the Response struct. The buffers contained in the Resposne struct must be
// freeded using tcp_server_client_cleanup. If an error occurs, an empty
// Response will be returned and this function will free any allocated resources.
Response tcp_server_process_request(Request request);

#endif
