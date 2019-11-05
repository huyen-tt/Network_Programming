#include <stdio.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define BUFF_SIZE 1024

char num[BUFF_SIZE];
char alpha[BUFF_SIZE];
int check;
void ReceiveBuff(char *receive){
    int numcount = 0;
            int alphacount =0; 
            memset(num,'\0',sizeof(num));
            memset(alpha,'\0', sizeof(alpha));
            for(int i = 0; i < strlen(receive); i++){
                if ((receive[i]>=65 && receive[i]<=90) || (receive[i]>=97 && receive[i]<=122)){
                    alpha[alphacount]= receive[i];
                    alphacount ++;
                }
                else if(receive[i]>=48 && receive[i]<=57){
                    num[numcount]= receive[i];
                    numcount ++;
                }
                else {
                    check = 1;
                    printf("Error!\n");
                    break;
                }
            }
}

int main(int argc, char *argv[]){
    
    int server_sock; 
    char buff[BUFF_SIZE];
    char error[BUFF_SIZE];
    char exit_loop[BUFF_SIZE];
    // char sending[BUFF_SIZE];
    int bytes_sent, bytes_received;
    struct sockaddr_in server; 
    struct sockaddr_in curr_client;
    struct sockaddr_in client1;
    struct sockaddr_in client2;
    memset(&server,0, sizeof(server));
    memset(&curr_client,0, sizeof(curr_client));
    memset(&client1,0, sizeof(client1));
    memset(&client2,0, sizeof(client2));

    char *err_connect = "2 client isn't connected!\n";
    int sin_size;
    short PORT;
    // int port = 0;
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
        printf("Port1: %d\n", client1.sin_port);
        printf("Port2: %d\n", client2.sin_port);
        printf("Current Port: %d\n", curr_client.sin_port);

        // sin_size2=sizeof(client2);
        bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &curr_client, &sin_size);
        memset(error,'\0', sizeof(error));
        memset(exit_loop,'\0', sizeof(exit_loop));
        check =0;
        // memset(sending,'\0', sizeof(sending));
        if (bytes_received < 0){
            perror("\nError: ");
            return 0;
        }
        else{
            buff[bytes_received] = '\0';
            if((curr_client.sin_port == client1.sin_port) && (client2.sin_port != 0)){// client1 send string to client2
                ReceiveBuff(buff);
                strcpy(exit_loop,"exit");
                // strcpy(sending,"send");
                // sendto(server_sock, sending, bytes_received, 0,(struct sockaddr *) &client1, sin_size);
                if(check == 1){
                    strcpy(error,"Error!");
                    sendto(server_sock,error, 8, 0, (struct sockaddr *) &client2, sin_size );
                    sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client2, sin_size);
                    check = 0;
                }
                else {
                    if(num[0] != '\0')
                        sendto(server_sock,num, bytes_received, 0, (struct sockaddr *) &client2, sin_size );
                    if(alpha[0] != '\0')
                        sendto(server_sock,alpha, bytes_received, 0, (struct sockaddr *) &client2, sin_size );
                    sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client2, sin_size);
                }
            }
            else if((curr_client.sin_port == client2.sin_port) && (client1.sin_port !=0)){//client2 send string to client1
                ReceiveBuff(buff);
                strcpy(exit_loop,"exit");
                // strcpy(sending,"send");
                // sendto(server_sock, sending, bytes_received, 0,(struct sockaddr *) &client2, sin_size);
                if(check == 1){
                    strcpy(error,"Error!");
                    sendto(server_sock,error, 8, 0, (struct sockaddr *) &client1, sin_size );
                    sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client1, sin_size);
                    check = 0;
                }
                else {
                    if(num[0] != '\0')
                        sendto(server_sock,num, bytes_received, 0, (struct sockaddr *) &client1, sin_size );
                    if(alpha[0] != '\0')
                        sendto(server_sock,alpha, bytes_received, 0, (struct sockaddr *) &client1, sin_size );
                    sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client1, sin_size);
                }

            }
            else if((client1.sin_port != 0) && (curr_client.sin_port != client1.sin_port) && (client2.sin_port ==0)){// client 2 send string to client 1
                client2 = curr_client;
                ReceiveBuff(buff);
                strcpy(exit_loop,"exit");
                // strcpy(sending,"send");
                // sendto(server_sock, sending, bytes_received, 0,(struct sockaddr *) &client1, sin_size);
                if(check == 1){
                    strcpy(error,"Error!");
                    sendto(server_sock,error, 8, 0, (struct sockaddr *) &client1, sin_size );
                    sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client1, sin_size);
                    check = 0;
                }
                else {
                    if(num[0] != '\0')
                        sendto(server_sock,num, bytes_received, 0, (struct sockaddr *) &client1, sin_size );
                    if(alpha[0] != '\0')
                        sendto(server_sock,alpha, bytes_received, 0, (struct sockaddr *) &client1, sin_size );
                    sendto(server_sock, exit_loop, 4, 0,(struct sockaddr *) &client1, sin_size);
                }

            }
            else {
                client1 = curr_client;
                sendto(server_sock,(const char*)err_connect, strlen(err_connect), 0, (struct sockaddr *)&client1, sin_size);
            }

        }
    }
    close(server_sock);
    return 0;
}