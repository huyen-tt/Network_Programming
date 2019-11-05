#include <stdio.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFF_SIZE 1024

int main(int argc, char *argv[]){
  int client_sock;
  char buff[BUFF_SIZE];
  struct sockaddr_in server_addr;
  int bytes_sent,bytes_received, sin_size;
  short SERV_PORT;
  char *SERV_IP;

  if (argc != 3) {
    fprintf(stderr, "Use command line parameters for the IP address and port number of server:\n %s <IPAddress> <PortNumber>\n", argv[0]);
    exit(1);
  }
  
  //Step 1: Construct a UDP socket
  SERV_IP = argv[1];
  SERV_PORT = atoi(argv[2]);
  
  if ((client_sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ){ 
    perror("\nError: ");
    return 0;
  }

  //Step 2: Define the address of the server
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERV_IP);
  server_addr.sin_port = htons(SERV_PORT);
 
  
  //Step 3: Communicate with server

  pid_t pid;
  pid = fork();

    if(pid == 0){
      do{
        while(1){
          bytes_received = recvfrom(client_sock, buff, BUFF_SIZE - 1, 0, (struct sockaddr *) &server_addr, &sin_size);
          if(bytes_received < 0){
            perror("Error: ");
            return 0;
          }
          buff[bytes_received] = '\0';
          if(strcmp(buff,"exit") == 0)
            break;
          printf("--> Reply: %s\n", buff);
        }
      }while(1);
      
    }
    else {
      do{
        printf("\nString to send:");
        fgets(buff, BUFF_SIZE, stdin);
        if(buff[0] == '\n') exit(1);
        buff[strlen(buff) - 1] = '\0';
        sin_size = sizeof(server_addr);
        
        bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
        if(bytes_sent < 0){
          perror("Error: ");
          // close(client_sock);
          return 0;
        }
      }while(1);
    }
  
    
  close(client_sock);
  return 0;
}
