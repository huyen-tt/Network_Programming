#include <stdio.h> /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define BACKLOG 20
#define BUFF_SIZE 1024

char filename[256] = "account.txt";

typedef struct Account{
    char h_userID[20];
    char h_password[20];
    int h_status;
    int h_isLogin;
    int h_countWrongPass; //count the number of wrong password entry
    struct Account *next;
} account;
account *head = NULL;
account *current = NULL;

void ReadData(){
    account acc;
    int i = 0;
    FILE *f;
    if (!(f = fopen(filename, "r"))){
        printf("\n File not found!! \n\n");
    }
    else{
        while (!feof(f)){   
            account *link = (account *)malloc(sizeof(account));
            fscanf(f, "%s %s %d", acc.h_userID, acc.h_password, &acc.h_status);
            acc.h_isLogin = 0;
            acc.h_countWrongPass = 0;
            //insert to head of linked list
            strcpy(link->h_userID, acc.h_userID);
            strcpy(link->h_password, acc.h_password);
            link->h_status = acc.h_status;
            link->next = head;
            head = link;
        }
    }
    fclose(f);
}

//Log in function
void LogIn(int conn_sock, char userID[], char password[]){
    int done = 0;
    do{
        account *ptr;
        ptr = head;
        int trueID = 0; // check UserID existed or not
        while (ptr != NULL){
            if (strcmp(userID, ptr->h_userID) == 0){
                trueID = 1;
                //check status
                if (ptr->h_status == 0){
                    char *msg = "Account was blocked\n";
                    send(conn_sock, msg, strlen(msg), 0);
                    printf("Account \"%s\" is blocked.\n", ptr->h_userID);
                }
                else{
                    //check password
                    if (strcmp(password, ptr->h_password) == 0){
                        done = 1;
                        char *msg = "Successful login. Press ENTER to log out\n";
                        send(conn_sock, msg, strlen(msg), 0);
                        printf("Successful login\n");
                        break;
                    }
                    else{
                        ptr->h_countWrongPass++;
                        char *msg = "Password is incorrect.\n";
                        if (ptr->h_countWrongPass == 3){
                            char *msg = "Password is incorrect 3 times. You are blocked\n";
                            send(conn_sock, msg, strlen(msg), 0);
                            printf("Password is incorrect 3 times. Account \"%s\" is blocked.\n", ptr->h_userID);
                            ptr->h_status = 0;
                            done = 1;
                            ptr->h_countWrongPass = 0;
                            //Update status from 1 to 0
                            FILE *f;
                            if (!(f = fopen(filename, "w"))){
                                printf("\n File not found!!\n");
                            }
                            else{
                                account *ptr;
                                ptr = head;
                                while (ptr != NULL)
                                {
                                    fprintf(f, "%s %s %d\n", ptr->h_userID, ptr->h_password, ptr->h_status);
                                    ptr = ptr->next;
                                }
                            }
                            fclose(f);
                        }
                        else{
                            send(conn_sock, msg, strlen(msg), 0);
                            printf("%s", msg); 
                        }   
                    }   
                }
            }
            ptr = ptr->next;
        }
        if (trueID == 0){
            char *msg = "This Account not exist!\n";
            send(conn_sock, msg, strlen(msg), 0);
            printf("Account \"%s\" is not exist.\n", userID);
            done = 1;
        }
    } while (done = 0);
}

int main(int argc, char const *argv[]){
    if (argc != 2){
        printf(stderr, "Run the port number according to the command line parameter as follows: %s <PortNumber>\n", argv[0]);
        return 1;
    }

    ReadData();
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    int listen_sock, conn_sock; /* file descriptors */
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    int sin_size;
    int pid;
   
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){ 
        perror("\nError: ");
        return 0;
    }
    
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));     
    server.sin_addr.s_addr = htonl(INADDR_ANY); 
    if (bind(listen_sock, (struct sockaddr *)&server, sizeof(server)) == -1){ 
        perror("\nErrow: ");
        return 0;
    }

    if (listen(listen_sock, BACKLOG) == -1){ 
        perror("\nError: ");
        return 0;
    }
    
    while (1){
        sin_size = sizeof(struct sockaddr_in);
        if ((conn_sock = accept(listen_sock, (struct sockaddr *)&client, &sin_size)) == -1)
            perror("\nError: ");
        
        pid = fork();
        if(pid > 0 ){
            close(conn_sock);
            continue;
        }
        else if (pid == 0){
            while(1){
                char userID[BUFF_SIZE];
                char password[BUFF_SIZE];
                char buff[BUFF_SIZE];
                memset(userID, 0, strlen(userID));
                memset(password, 0, strlen(password));
                recv(conn_sock, userID, BUFF_SIZE, 0);
                send(conn_sock, "userID\n", sizeof("userID\n"), 0);
                userID[strlen(userID) - 1] = '\0';
                printf("UserID: %s\n", userID);
                recv(conn_sock, password, BUFF_SIZE, 0);
                send(conn_sock, "password\n", sizeof("password\n"), 0);
                password[strlen(password) - 1] = '\0';
                printf("Password: %s\n", password);
                sleep(1);
                LogIn(conn_sock, userID, password);
                printf("\n");
            }
            close(conn_sock);
            break;
        }
    }
    close(listen_sock);
    return 0;
}