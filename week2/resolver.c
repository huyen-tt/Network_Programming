#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Classify input value is domain name or IP address
classify(char* input){
  int i;
  for (i=0; i<strlen(input); i++){
    if ((input[i]>48 && input[i]<57)|| input[i]=='.')
      return 1; // input is IP address
    else return 2; // input is domain name
  }

}

main(){
  char input[25];
  printf("Please input parameter:\n");
  gets(input);

  if (classify(input)==2){
    struct hostent *a;
    struct in_addr **b;
    int i;

    a = gethostbyname(input);
    if (a == NULL)
      printf("Not found information\n");
    else{
      b = (struct in_addr**)a->h_addr_list;
      printf("Official IP: %s\n", inet_ntoa(*b[0]));
      for (i=1; b[i]!=NULL; i++){
      printf("Alias IP: %s\n", inet_ntoa(*b[i]));
      }
    }
  } else{
      struct hostent *a;
      struct in_addr **b;
      struct in_addr ipv4;
      struct in6_addr ipv6;
      struct in_addr *add;

      int i;
      if (inet_pton(AF_INET, input, &ipv4)){
        a = gethostbyaddr(&ipv4, sizeof(ipv4), AF_INET);
        if (a==NULL)
          printf("Not found information\n");
        else{
          printf("Official name: %s\n", a->h_name);
          printf("Alias name : \n");
          for (i = 0; a->h_aliases[i] != NULL; i++){
            printf("%s \n", a->h_aliases[i]);
          }
        }
      }
    }
}
