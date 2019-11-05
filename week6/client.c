#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

int main(int argc, char const *argv[]){   
    int client_sock;
    struct sockaddr_in server_addr; 
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (argc != 3){
        printf(stderr, "Use command line parameters for the IP address and port number of server:\n %s <IPAddress> <PortNumber>\n", argv[0]);
        return 1;
    }
    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
        printf("\nError!Can not connect to sever!");
        return 0;
    }
    while (1)
    {
        char userID[BUFF_SIZE];
        char password[BUFF_SIZE];
        char buff[BUFF_SIZE];
        char systemMsg[BUFF_SIZE];
        int systemMsg_bytes_received;
        int bytes_sent;
        int bytes_received;
      
        //send userID to server
        printf("----- Log in -----\n");
        printf("UserID:");
        memset(userID, '\0', (strlen(userID) + 1));
        fgets(userID, BUFF_SIZE, stdin);
        bytes_sent = send(client_sock, userID, strlen(userID), 0);
        bytes_received = recv(client_sock, buff, BUFF_SIZE - 1, 0);
        buff[bytes_received] = '\0';

        //send password to server
        printf("Password:");
        memset(password, '\0', (strlen(password) + 1));
        fgets(password, BUFF_SIZE, stdin);
        bytes_sent = send(client_sock, password, strlen(password), 0);
        bytes_received = recv(client_sock, buff, BUFF_SIZE - 1, 0);
        buff[bytes_received] = '\0';

        systemMsg_bytes_received = recv(client_sock, systemMsg, BUFF_SIZE - 1, 0);
        if (systemMsg_bytes_received <= 0)
        {
            printf("\nError!Cannot receive data from sever!\n");
            break;
        }
        systemMsg[systemMsg_bytes_received] = '\0';
        printf("%s\n", systemMsg);
        if (strcmp(systemMsg, "Successful login. Press ENTER to log out\n") == 0)
        {   
            getchar();
        }
    }

    //Step 4: Close socket
    close(client_sock);
    return 0;
}