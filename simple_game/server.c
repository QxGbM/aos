/* 
 *  Project: Simple Game
 *  Team: 810
 *  Part: Liang Wei
 *  
 *  It's a classic server-client program using TCP/IP socket API.
 *  I'm not gonna cover the details.
 *  
 *  Usage:
 *    $ echo -1 > log.data
 *    $ gcc server.c & ./a.out 2468
 *    
 *    It should be runnable on Linux and maybe MacOS.
 *    And remember it should be run on your server, not the Arduino. 
 */



#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFSIZE 256
#define PORTNO 2468



void RaiseError(char *msg){
	perror(msg);
	exit(1);
}

char recvBuffer[BUFSIZE];
char sendBuffer[BUFSIZE];

int main( int argc, char *argv[]){
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if(argc < 2){
		fprintf(stderr, "ERROR: NO PORT PROVIDED.\n");
		exit(1);
	}
	
	// Socket file descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sockfd < 0 )
		RaiseError("socket() failed.");
	
	// Construct address struct
	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	serv_addr.sin_port = htons(portno);
	
	// Bind
	if( bind(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
		RaiseError("bind() failed.");

	// Listen
	listen(sockfd, 5);
	
	// Accept connection
	printf("Waiting for connection ...\n");
	clilen = sizeof(cli_addr);	
	while(1){
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if( newsockfd < 0 )
			RaiseError("accept() failed.");
	
		// Read from client
		memset((char*)recvBuffer, 0, sizeof(recvBuffer));
		if( read(newsockfd, recvBuffer, sizeof(recvBuffer)-1) < 0)
			RaiseError("read() failed.");
		
		// Client fetching the history
		if( strcmp(recvBuffer, "history") == 0  ){
			printf("Fetching history ...\n");
			
			int highPoint = 0;
			
			// read history score from log file
			FILE *fptr = fopen("log.data", "r+");
			fscanf(fptr, "%d", &highPoint);
			fclose(fptr);
		
			// prepare the send buffer
			sprintf(sendBuffer, "%d", highPoint);

			if( write(newsockfd, sendBuffer, sizeof(sendBuffer)) < 0 )
				RaiseError("write() failed.");


			continue;
		}
		
		// Client updating the score
		int curPoint = atoi(recvBuffer);
		unsigned int highPoint = 0;
		
		FILE *fptr = fopen("log.data", "r");
		fscanf(fptr, "%u", &highPoint);
		fclose(fptr);

		if( highPoint > curPoint ){
			printf("high = %u, cur = %d, updated.\n", highPoint, curPoint);
			
			// update the log file
			FILE *fptr = fopen("log.data", "w");
			fprintf(fptr, "%d\n", curPoint);
			fclose(fptr);
			
			// prepare the send buffer
			// because current score is faster than history score
			// so server need to notify the client that
			// "I will update it"
			memset((char*)sendBuffer, 0, sizeof(sendBuffer));
			sprintf(sendBuffer, "%d", 1);
			printf("%s\n", sendBuffer);

			// send to client
			if( write(newsockfd, sendBuffer, sizeof(sendBuffer)) < 0 )
				RaiseError("write() failed.");

		}else{
			printf("high = %u, cur = %d, not updated\n", highPoint, curPoint);

			// prepare the send buffer
			// because the current score is not faster than history score
			// so server need to notify the client that
			// "I will not update it"
			memset((char*)sendBuffer, 0, sizeof(sendBuffer));
			sprintf(sendBuffer, "%d", 0);	
			printf("%s\n", sendBuffer);

			// send to client
			if( write(newsockfd, sendBuffer, sizeof(sendBuffer)) < 0 )
				RaiseError("write() failed.");


		}

		
	}	
	// Close
	close(sockfd);
	close(newsockfd);
	return 0;
}
