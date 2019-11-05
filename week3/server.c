#include <stdio.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define BUFF_SIZE 1024

int main(int argc, char *argv[])
{
    char num[BUFF_SIZE];
    char alpha[BUFF_SIZE];
    int server_sock; 
    char buff[BUFF_SIZE];
    char error[BUFF_SIZE];
    char exit_loop[BUFF_SIZE];
    int bytes_sent, bytes_received;
    struct sockaddr_in server; 
    struct sockaddr_in client; 
    int sin_size;
    short PORT;
    int check = 0;

    if (argc != 2) {
        fprintf(stderr, "Run the port number according to the command line parameter as follows: %s <PortNumber>\n", argv[0]);
        exit(1);
    }

    //Step 1: Construct a UDP socket
    PORT = atoi(argv[1]);
    if ((server_sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) <0 ){  /* calls socket() */
        perror("\nError: ");
        return 0;
    }
    
    //Step 2: Bind address to socket
    bzero(&server, sizeof(server)); 
    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = htonl(INADDR_ANY);        
    server.sin_port = htons(PORT);  
    if(bind(server_sock,(struct sockaddr*)&server,sizeof(server))<0){ /* calls bind() */
        perror("\nError: ");
        return 0;
    }     
    
    //Step 3: Communicate with clients
    while(1){
        sin_size=sizeof(client);
        bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client, &sin_size);
        memset(error,'\0', sizeof(error));
        memset(exit_loop,'\0', sizeof(exit_loop));
        if (bytes_received < 0){
            perror("\nError: ");
            return 0;
        }
        else{
            buff[bytes_received] = '\0';
            printf("Receive from client: %s\n", buff);
            int numcount = 0;
            int alphacount =0; 
            memset(num,'\0',sizeof(num));
            memset(alpha,'\0', sizeof(alpha));
            for(int i = 0; i < strlen(buff); i++){
                if ((buff[i]>=65 && buff[i]<=90) || (buff[i]>=97 && buff[i]<=122)){
                    alpha[alphacount]= buff[i];
                    alphacount ++;
                }
                else if(buff[i]>=48 && buff[i]<=57){
                    num[numcount]= buff[i];
                    numcount ++;
                }
                else {
                    check = 1;
                    printf("Error!\n");
                    break;
                }
            }
            // strcpy(exit_loop,"exit");
            if(check == 1){
                strcpy(error,"Error!");
                sendto(server_sock,error, 8, 0, (struct sockaddr *) &client, sin_size );
                strcpy(exit_loop,"exit");
                sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client, sin_size);
                check = 0;
            }
            else {
                if(num[0] != '\0')
                    sendto(server_sock,num, bytes_received, 0, (struct sockaddr *) &client, sin_size );
                if(alpha[0] != '\0')
                    sendto(server_sock,alpha, bytes_received, 0, (struct sockaddr *) &client, sin_size );
                strcpy(exit_loop,"exit");
                sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client, sin_size);
            }
        }
                        
    }
    
    close(server_sock);
    return 0;
}