#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct user{
  char username[20];
  char password[20];
  int status;
  int wrongTimes;
  int isSignin;
} user;

typedef struct node{
  struct user account;
  struct node* next;
} node;
 


//Add a node at the end, return current node
node *append(node *current, user data){
    node *new_node;
    new_node = (node*)malloc(sizeof(node));

    new_node->account = data;
    new_node->next = NULL;
    current->next = new_node; 
    current = current->next; 
    return current;
}

//Use Username to search and return a node, return NULL if not found
node *searchNode(node *root, char username[20]){
	node *s_node;
	s_node = (node*)malloc(sizeof(node));

	s_node = root;
	while (s_node->next!=NULL){
		s_node=s_node->next;
		if (strcmp((s_node->account).username, username) == 0) return s_node;
	}
	return NULL;
}


//Read data from file account.txt and insert to link list, return current node
node *readAccountFile(node *current){
	user data;
	FILE *file;

	if((file = fopen("account.txt", "r")) == NULL){
		printf("Cannot open file!\n");
		exit(0);
	} else{
		while(fscanf(file,"%s",data.username)!=EOF){
        fscanf(file,"%s",data.password);
        fscanf(file,"%d", &data.status);
        data.isSignin = 0;
        data.wrongTimes = 0;
        current = append(current,data);
    	}
	fclose(file);
    return current;
	}
}


//Write data from link list to file
void updateAccountFile(node *root){
	FILE *file;
	file = fopen("account.txt","w");
	node *u_node;
	u_node = (node*)malloc(sizeof(node));

	u_node = root;
	while (u_node->next!=NULL){
		u_node=u_node->next;
		fprintf(file, "%s %s %d\n",(u_node->account).username,(u_node->account).password,(u_node->account).status);
	}
	fclose(file);
}


//Register new Account and update to link list and file, return node current
node *creatNewAccount(node *current, node *root){
	user newAccount;

	printf("New username: "); 
	fflush(stdin); scanf("%s",newAccount.username);
	if(searchNode(root, newAccount.username)!=NULL){
		printf("\nAccount existed!\n");
		return current;
	}
	printf("New password: "); 
	fflush(stdin); scanf("%s",newAccount.password);
	newAccount.status = 1;
	newAccount.isSignin = 0;
	newAccount.wrongTimes = 0;
	current = append(current, newAccount);
	printf("\nSuccessful registration!\n");
	updateAccountFile(root);
	return current;	
}

//Sign in and update to link list and file
void signinAccount(node *root){
	user account;
	node *s_node;
	s_node = (node*)malloc(sizeof(node));

	printf("Username: "); 
	fflush(stdin); scanf("%s",account.username);
	s_node = searchNode(root, account.username);
	if(s_node == NULL){
		printf("\nCannot find account!\n");
	} else if((s_node->account).status == 0){
		printf("\nAccount is blocked!\n");
	} else{
		printf("Password: "); 
		fflush(stdin); scanf("%s",account.password);
		if(strcmp((s_node->account).password, account.password) != 0){
			if((s_node->account).wrongTimes < 3){
				printf("\nPassword is incorrect!\n");
				(s_node->account).wrongTimes++;
			} else{
				printf("\nPassword is incorrect. Account is blocked!\n");
				(s_node->account).status = 0;
				updateAccountFile(root);
			}		
		} else{
			printf("\nHello %s!\n", account.username);
			(s_node->account).wrongTimes = 0;
			(s_node->account).isSignin = 1;
		}
	}	
}

//Search Account
void searchAccount(node *root){
	user account;
	node *s_node;
	s_node = (node*)malloc(sizeof(node));

	printf("Search username: ");
	fflush(stdin); scanf("%s",account.username);
	s_node = searchNode(root, account.username);
	if(s_node == NULL){
		printf("\nCannot find account!\n");
	} else{
		if((s_node->account).status == 0) printf("\nAccount is blocked!\n");
		else printf("\nAccount is active!\n");
	}	
}

//Log out
void logoutAccount(node *root){
	user account;
	node *l_node;
	l_node = (node*)malloc(sizeof(node));

	printf("Username: "); 
	fflush(stdin); scanf("%s",account.username);
	l_node = searchNode(root, account.username);
	if(l_node == NULL){
		printf("\nCannot find account!\n");
	} else {
		if((l_node->account).isSignin == 1) {
			printf("\nGoodbye %s!\n", account.username);
			(l_node->account).isSignin = 0;
		} else printf("\nAccount is not sign in!\n");
	}
}

int main(){
	node *root, *current;
	root = (node*)malloc(sizeof(node));
	root->next = NULL;
	current = (node*)malloc(sizeof(node));
	current = root;
	int userChoice = 0;

	current = readAccountFile(current);
	do{

		int userChoice;

		printf("\n\nUSER MANAGEMENT PROGRAM\n");
		printf("---------------------------------------------\n");
		printf("   1. Register\n");
		printf("   2. Sign in\n");
		printf("   3. Search\n");
		printf("   4. Sign out\n");
		printf("Your choice (1-4, other to quit):");
		fflush(stdin);
		scanf("%d",&userChoice);

		switch(userChoice){
			case 1:
				printf("\n");
				current = creatNewAccount(current,root);
				break;
			case 2:
				printf("\n");
				signinAccount(root);
				break;
			case 3:
				printf("\n");
				searchAccount(root);
				break;
			case 4:
				printf("\n");
				logoutAccount(root);
				break;
			default:
				break;
		}
	} while(userChoice == 1 || userChoice == 2 || userChoice == 3 || userChoice == 4);
	return 0;
}