/*standard symbols*/
#include <unistd.h>

/* sockets */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

/* string / errors */
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define SERV_PORT       8080
#define SERV_HOST_ADDR  "192.168.1.6"
#define BUF_SIZE        100
#define BACKLOG         5

int main(int argc, char* argv[]) {
    int sockfd, connfd;
    unsigned int len;
    struct sockaddr_in servaddr, client;

    int len_rx, len_tx = 0;
    char buff_tx[BUF_SIZE] = "Hello client, Iam the server";
    char buff_rx[BUF_SIZE];

    /* socket creation */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1){
        fprintf(stderr, "[SERVER-error]: socket creation failed. %d: %s \n");
        return -1;
    }else{
        printf("[SERVER]: Socket successfully created..\n");
    }

    /* clear structure */
    memset(&servaddr, 0, sizeof(servaddr));

    /* assign IP, SERV_PORT, IPV4 */
    servaddr.sin_family         = AF_INET;
    servaddr.sin_addr.s_addr    = inet_addr(SERV_HOST_ADDR);
    servaddr.sin_port           = htons(SERV_PORT);

    /* Bind socket */
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0){
        fprintf(stderr, "[SERVER-error]: socket bind failed. %d: %s \n");
        return -1;
    }else{
        printf("[SERVER]: Socket successfully binded \n");
    }

    /* Listen */
    if ((listen(sockfd, BACKLOG)) != 0){
        fprintf(stderr, "[SERVER-error]: socket listen failed. %d: %s \n");
        return -1;
    }else{
        printf("[SERVER]: Listening on SERV_PORT %d \n\n");
    }

    len = sizeof(client);

    /* Accept the data from incoming sockets in a iterative way */
    while(1){
        connfd = accept(sockfd, (struct sockaddr *)&client, &len);
        if (connfd < 0){
            fprintf(stderr, "[SERVER-error]: connection not accepted. %d: %s \n", errno, strerror(errno));
            return -1;
        }else{
            while(1) { /* read data from a client socket till it is closed */
                /* read client message, copy it into buffer */
                len_rx = read(connfd, buff_rx, sizeof(buff_rx));

                if(len_rx == -1){
                    fprintf(stderr, "[SERVER-error]: connfd cannot be read. %d: %s \n", errno, strerror(errno));
                }else if(len_rx == 0){
                    printf("[SERVER]: client socket closed \n\n");
                    close(connfd);
                    break;
                }else{
                    write(connfd, buff_tx, strlen(buff_tx));
                    printf("[SERVER]: %s \n", buff_rx);
                }
            }
        }
    }
}
