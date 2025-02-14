#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct node{
	char *username;
	char *password;
	struct node *next;
};

struct node *head = NULL;

struct node *CreateNode(char *name, char *pass){
	struct node *newnode = (struct node *)malloc(sizeof(struct node));
	newnode->username = name;
	newnode->password = pass;
	newnode->next = NULL;
	return newnode;
}

void AddNode(char *name, char *pass){
	
	if(head == NULL){
		head = CreateNode(name, pass);
		return;
	}
	
	else{
		struct node *temp = head;
		while(temp->next != NULL){
			
			temp = temp->next;
			
		}
		temp->next = CreateNode(name, pass);
	}
}

void FindNode(char *name){
	struct node *temp = head;
	
	if(head == NULL){
		
		printf("Empty List.\n");
		
	}
	
	while(temp->next!=NULL){
		
		if(strcmp(temp->username, name) == 0){
			
			printf("Pass: %s\n", temp->password);
			return;
			
		}
	}

	printf("Username does not exist\n");

}

void Print(){
	
	struct node *temp = head;
	
	while(temp->next != NULL){
		
		printf("Username: %s\nPassword: %s\n\n", temp->username, temp->password);
		temp = temp->next;
		
	}
		printf("Username: %s\nPassword: %s\n\n", temp->username, temp->password);
		return;
}

void RemoveAll(){
	
	struct node *temp = head;
	struct node *tempPrev = temp;
	
	while(temp->next != NULL){
		
		temp = temp->next;
		free(tempPrev);
		tempPrev = temp;
	}
	
	free(temp);

}

void RemoveOne(char *name){
	struct node *temp = head;
    struct node *tempPrev = temp;

        while(temp->next != NULL){
			
                if(strcmp(temp->username, name) == 0){	
				printf("Removing: %s\n\n", name);
				tempPrev->next = temp->next;
				free(temp);
				return;
		}
		
		else{
			
			tempPrev = temp;
			temp = temp->next;
	    	 
		}
     }
}
