#include <netdb.h>
#include <stdint.h>

typedef int (* resp_callback)(int status, char* resp_body);

int send_to_socket(int sockfd, char *req);
int read_from_socket(int sockfd, char *resp, int buffer_size);
int get(char *uri, resp_callback callback);
int post(char *uri, char *body, resp_callback callback);
int parse_uri(char *src, struct hostent **host, uint16_t *port, char *tail);
int connect_to_socket(struct hostent *host, uint16_t port);
