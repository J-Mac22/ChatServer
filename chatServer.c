#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "list.h"

#define clientnum 10
#define maxlogininfo 10
 

int thrdarr[clientnum]= {0};
char buffer[1024];
pthread_mutex_t mylock;

struct connection_t{
	int clientSocket;
	int idx;
	struct sockaddr clientAddress;
	socklen_t clientAddressLength;
};

void Death(){
	
	while(1){
		
		sleep(10);
		
	}
}


void *client(void *ptr){

	struct connection_t *conn;
	conn = (struct connection_t *)ptr;
	int thidx = conn->idx;
	thrdarr[thidx] = 1;
	int len;

	//Holding username and password
	char *UserName;
	char *PassWord;
	char un[20];
	char pw[20];
	char *toSend;

	//Accept a username input
	 
    memset(buffer, 0, sizeof(buffer));
    if ( (len = recv(conn->clientSocket, buffer, sizeof(buffer), 0)) == -1) {
			
                printf("len = %d\n", len);
                perror("Failed to receive message.");
                exit(EXIT_FAILURE);
				
        }

	printf("Recieved successfully %s\n", buffer);

	//Traverse through the information
	for(struct node *temp = head; temp != NULL; temp = temp->next){

		if(strcmp(buffer, temp->username) == 0){
			
			printf("We have a valid username input: %s\n", buffer);

			 
				
			 
		}
		
		if(temp->next == NULL){
			
			//Adding username to linked list
			UserName = buffer;  
			strcpy(un, buffer);
			 
			toSend = "This username does not exist, please make a new Password";
			
                    if(send(conn->clientSocket, toSend, (8*sizeof(toSend)), 0) == -1){
						
                             printf("Error\n");
                             exit(EXIT_FAILURE);
							 
                        }

			 

			strcpy(pw, buffer);
			AddNode(un, pw);
			break;  
		}
	}

	 
 
	Print();
	toSend = "Login Was Successful";
	
        if(send(conn->clientSocket, toSend, (2*sizeof(toSend)), 0) == -1){
			
                printf("Error\n");
                exit(EXIT_FAILURE);
				
        }

	while(1){
		 
		memset(buffer, 0, sizeof(buffer));
		if ((len = recv(conn->clientSocket, buffer, sizeof(buffer), 0)) == -1) {  
		
			printf("len = %d\n", len);
			perror("Failed to receive message.");
			exit(EXIT_FAILURE);
			
		}
		
		else { 
	 
	        printf("%d-%s: %s\n", thidx, un, buffer);
			
		}

		if(strcmp(buffer, "EXIT") == 0){
			
			memset(buffer, 0, sizeof(buffer));
			break;
			
		}

		sleep(2);

	} 

	close(conn->clientSocket);
    printf("Client socket closed.\n");
	free(conn);
} 



int main(){

	pthread_mutex_init(&mylock, NULL);
	struct connection_t *connt;
	printf("Server started.\n");
	const char *portNumber = "8080";
    const int backlog = 2;
    int serverSocket;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    struct addrinfo *results;
    struct addrinfo *record;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
	pthread_t th[10];
	struct connection_t * connect;
	const size_t bufferSize = 1024;

    	if ((getaddrinfo(NULL, portNumber, &hints, &results)) != 0) {
			
        	perror("Failed to translate server socket.");
	        exit(EXIT_FAILURE);
			
    	}

    	printf("Server socket translated.\n");

    	for(record = results; record != NULL; record = record->ai_next) {
			
	        serverSocket = socket(record->ai_family, record->ai_socktype, record->ai_protocol);
        	if (serverSocket == -1) continue;
			int enable = 1;
        	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        	if (bind(serverSocket, record->ai_addr, record->ai_addrlen) == 0) break;
        	close(serverSocket);
			
    	}



    if (record == NULL) {
		
        exit(EXIT_FAILURE);
		
    }

    freeaddrinfo(results);
    printf("Server socket created and bound.\n");

    if (listen(serverSocket, backlog) == -1) {
		
        exit(EXIT_FAILURE);
		
    }

    AddNode("Ramapo", "123");
 
    printf("Server started listening.\n");
	int id = 1;
	int i = 1;

    while (1) {

        	printf("Server still running.\n");
		    connect = (struct connection_t *)malloc(sizeof(struct connection_t));

        	if ((connect->clientSocket = accept(serverSocket, &connect->clientAddress, &connect->clientAddressLength)) < 0) {
				
        	    	perror("Failed to accept client socket.");
        	    	exit(EXIT_FAILURE);
					
        	}

        	printf("Client socket accepted.\n");

	

		while(id < clientnum){
			
			if(thrdarr[id] == 0){
				
				connect->idx = id;
				pthread_create(&th[id], NULL, client, (void *)connect);
				break;
				
			}
			
			id++;
			
		}
		
		printf("Main While Loop -> just created a thread\n");
		sleep(1);
	}

        for(; i<10; i++){
			   
			printf("About to Join th[i]: %d\n", i);
            pthread_join(th[i], NULL);
			printf("Joined\n");
			
         }

	printf("About to Loop to top of main while loop\n");
	pthread_mutex_destroy(&mylock);
    return 0;
}

