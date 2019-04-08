#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "http.h"

#define REQ_BUFFER 1024
#define RESP_BUFFER 8192

int get(char *uri, resp_callback callback) {
    char req[REQ_BUFFER] = "";

    char url[128] = "";
    uint16_t port = 80;
    struct hostent *server = NULL;
    int err = parse_uri(uri, &server, &port, url);
    if (err != 0) {
        return 1;
    }

    char headers[] = "";

    char method[] = "GET";
    char version[] = "HTTP/1.0";
    if (snprintf(req, REQ_BUFFER, "%s %s %s\r\n%s\r\n",
        method, url, version, headers) == -1) {
        
        return 1;
    }
    
    int sockfd = connect_to_socket(server, port);

    send_to_socket(sockfd, req);

    char response[RESP_BUFFER] = "";
    memset(response, 0, sizeof(response));
    read_from_socket(sockfd, response, sizeof(response));

    close(sockfd);

    int code = 0;
    if (sscanf(response, "%s %d", version, &code) != 2) {
        return 1;
    }

    char *end_of_head = strstr(response, "\r\n\r\n");
    if (strlen(end_of_head) < 4) {
        return 1;
    }

    callback(code, end_of_head + 4);

    return 0;
}

int post(char *uri, char *body, resp_callback callback) {
    char req[REQ_BUFFER] = "";

    char url[128] = "";
    uint16_t port = 80;
    struct hostent *server = NULL;
    int err = parse_uri(uri, &server, &port, url);
    if (err != 0) {
        return 1;
    }

    char headers[1024] = "";
    snprintf(headers, 1024,
        "Content-Type:application/x-www-form-urlencoded\r\nContent-Length:%lu\r\n",
        strlen(body));

    char method[] = "POST";
    char version[] = "HTTP/1.0";
    if (snprintf(req, REQ_BUFFER, "%s %s %s\r\n%s\r\n%s\r\n",
        method, url, version, headers, body) == -1) {
        
        return 1;
    }
    
    int sockfd = connect_to_socket(server, port);

    send_to_socket(sockfd, req);

    char response[RESP_BUFFER] = "";
    memset(response, 0, sizeof(response));
    read_from_socket(sockfd, response, sizeof(response));

    close(sockfd);

    int code = 0;
    if (sscanf(response, "%s %d", version, &code) != 2) {
        return 1;
    }

    char *end_of_head = strstr(response, "\r\n\r\n");
    if (strlen(end_of_head) < 4) {
        return 1;
    }

    callback(code, end_of_head + 4);

    return 0;
}

int send_to_socket(int sockfd, char *req) {
    int total = strlen(req);
    int sent = 0;
    int bytes = 0;
    do {
        bytes = write(sockfd, req + sent, total - sent);
        if (bytes < 0) {
            printf("ERROR writing message to socket\n");
        }
        if (bytes == 0) {
            break;
        }
        sent += bytes;
    } while (sent < total);

    return sent;
}

int read_from_socket(int sockfd, char *resp, int buffer_size) {
    int total = buffer_size - 1;
    int received = 0;
    int bytes = 0;
    do {
        bytes = read(sockfd, resp + received, total - received);
        if (bytes < 0) {
            printf("ERROR reading response from socket\n");
        }
        if (bytes == 0) {
            break;
        }
        received += bytes;
    } while (received < total);

    if (received == total) {
        printf("ERROR storing complete response from socket\n");
    }

    return received;
}

int parse_uri(char *src, struct hostent **server, uint16_t *port, char *tail) {
    *port = 80;

    char sock[128] = "";
    char host[128] = "";
    if (sscanf(src, "http://%128[^/]%128[^\n]", sock, tail) == 0) {
        return 1;
    }

    if (sscanf(sock, "%[^:]:%hu", host, port) == 0) {
        return 1;
    }

    *server = gethostbyname(host);
    if (server == NULL) {
        printf("ERROR, no such host");
        return 1;
    }

    return 0;
}

int connect_to_socket(struct hostent *host, uint16_t port) {
    struct sockaddr_in serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    memcpy(&serv_addr.sin_addr.s_addr, host->h_addr, host->h_length);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket\n");
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting\n");
    }

    return sockfd;
}
